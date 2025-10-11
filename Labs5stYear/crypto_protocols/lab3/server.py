import json
import os
import socket
import sys
import threading
import time

# Allow running as `python lab3/server.py`
sys.path.insert(0, os.path.dirname(__file__))

from dh import (
    compute_kdf_sha256,
    compute_public_key,
    derive_shared_secret,
    generate_private_key,
    int_to_hex,
    make_parameters,
    parse_int,
    validate_parameters,
    validate_public_key,
)
from rfc3526 import get_group, list_groups


class DHSession:
    def __init__(self):
        self.params = get_group('group14')
        self._generate_ephemeral()
        self.last_peer_public = None
        self.last_server_public = None
        self.last_shared_hex = None
        self.last_shared_kdf = None
        self.created_at = time.time()

    def _generate_ephemeral(self):
        self.private_key = generate_private_key(self.params.q)
        self.public_key = compute_public_key(self.params.g, self.private_key, self.params.p)

    def set_params(self, params):
        if not validate_parameters(params):
            raise ValueError("Invalid parameters")
        self.params = params
        self._generate_ephemeral()

    def to_dict(self):
        d = self.params.as_dict()
        d["server_public"] = str(self.public_key)
        return d

    def to_hex_dict(self):
        return {
            "name": self.params.name,
            "p": int_to_hex(self.params.p),
            "g": int_to_hex(self.params.g),
            "q": int_to_hex(self.params.q),
            "server_public": int_to_hex(self.public_key),
        }

    def compute_secret(self, peer_public: int):
        if not validate_public_key(peer_public, self.params):
            raise ValueError("Peer public key not in subgroup")
        server_public_used = self.public_key
        secret = derive_shared_secret(peer_public, self.private_key, self.params)
        self.last_peer_public = peer_public
        self.last_server_public = server_public_used
        self.last_shared_hex = int_to_hex(secret)
        self.last_shared_kdf = compute_kdf_sha256(secret)
        # Rotate server private/public for next exchange (ephemeral behaviour)
        self._generate_ephemeral()
        return secret

    def last_status(self) -> str:
        if self.last_shared_hex is None:
            return "No shared secret derived yet"
        return (
            "Last shared secret available.\n"
            f"Server public used: {self.last_server_public}\n"
            f"Peer public: {self.last_peer_public}\n"
            f"Shared secret (hex): {self.last_shared_hex}\n"
            f"SHA256: {self.last_shared_kdf}\n"
        )


class DiffieHellmanServer:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port

    def start(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((self.host, self.port))
        sock.listen(5)
        print(f"Diffie-Hellman server on {self.host}:{self.port}")
        print("Use telnet/nc or client.py to interact. Default group: RFC 3526 group14.")
        try:
            while True:
                client, addr = sock.accept()
                print("Connection from", addr)
                thread = threading.Thread(target=self.handle_client, args=(client,), daemon=True)
                thread.start()
        except KeyboardInterrupt:
            print("Stopping server...")
        finally:
            sock.close()

    def handle_client(self, client_socket: socket.socket):
        session = DHSession()
        try:
            client_socket.sendall(
                (
                    "Diffie-Hellman (RFC 2631) Lab Server\n"
                    "Commands:\n"
                    "  HELP                         - Show this help\n"
                    "  PARAMS [JSON|HEX]            - Show domain parameters and server public key\n"
                    "  RFC <group>                  - Load RFC 3526 group (options: " + ', '.join(list_groups()) + ")\n"
                    "  GENERATE <bits>              - Generate new safe prime parameters (>=256 bits)\n"
                    "  PUB <value>                  - Send client public value (decimal or 0x hex)\n"
                    "  SECRET                       - Show last derived secret (if any)\n"
                    "  KDF                          - Show SHA-256 of last shared secret\n"
                    "  RESET                        - Regenerate server private/public\n"
                    "  STATUS                       - Show summary\n"
                    "  QUIT                         - Close connection\n\n"
                ).encode('utf-8')
            )

            while True:
                client_socket.sendall(b"> ")
                data = client_socket.recv(8192)
                if not data:
                    break
                line = data.decode('utf-8', errors='ignore').strip()
                if not line:
                    continue
                parts = line.split()
                cmd = parts[0].upper()

                try:
                    if cmd == 'QUIT':
                        break
                    elif cmd == 'HELP':
                        client_socket.sendall(b"Type commands as listed in the banner.\n")
                    elif cmd == 'PARAMS':
                        fmt = parts[1].upper() if len(parts) > 1 else 'DEC'
                        if fmt == 'JSON':
                            payload = session.to_dict()
                            payload['server_public'] = str(session.public_key)
                            client_socket.sendall((json.dumps(payload) + "\n").encode('utf-8'))
                        elif fmt == 'HEX':
                            payload = session.to_hex_dict()
                            msg_lines = [f"{k.upper()}: {v}" for k, v in payload.items()]
                            client_socket.sendall(("\n".join(msg_lines) + "\n").encode('utf-8'))
                        else:
                            params = session.params
                            msg = (
                                f"Parameters: {params.name} ({params.size_bits()} bits)\n"
                                f"P: {params.p}\n"
                                f"G: {params.g}\n"
                                f"Q: {params.q}\n"
                                f"SERVER_PUBLIC: {session.public_key}\n"
                            )
                            client_socket.sendall(msg.encode('utf-8'))
                    elif cmd == 'RFC' and len(parts) == 2:
                        group_name = parts[1]
                        params = get_group(group_name)
                        session.set_params(params)
                        client_socket.sendall(
                            (f"Loaded {params.name}; regenerated server public key.\n").encode('utf-8')
                        )
                    elif cmd == 'GENERATE' and len(parts) == 2:
                        try:
                            bits = int(parts[1])
                        except ValueError:
                            client_socket.sendall(b"Bits must be integer\n")
                            continue
                        if bits < 256:
                            client_socket.sendall(b"Bits must be >= 256\n")
                            continue
                        params = make_parameters(bits)
                        session.set_params(params)
                        client_socket.sendall(
                            (f"Generated safe-prime group {bits} bits; server key rotated.\n").encode('utf-8')
                        )
                    elif cmd == 'PUB' and len(parts) >= 2:
                        value_str = parts[1]
                        try:
                            peer_public = parse_int(value_str)
                        except ValueError:
                            client_socket.sendall(b"Invalid integer format\n")
                            continue
                        try:
                            secret = session.compute_secret(peer_public)
                        except ValueError as exc:
                            client_socket.sendall((f"ERROR: {exc}\n").encode('utf-8'))
                            continue
                        secret_hex = session.last_shared_hex
                        kdf = session.last_shared_kdf
                        # kdf = kdf[:-1] + "0"
                        msg = (
                            "OK\n"
                            f"SHARED_SECRET_HEX: {secret_hex}\n"
                            f"SHA256: {kdf}\n"
                            "Server key rotated for next exchange.\n"
                        )
                        client_socket.sendall(msg.encode('utf-8'))
                    elif cmd == 'SECRET':
                        if session.last_shared_hex is None:
                            client_socket.sendall(b"No shared secret yet.\n")
                        else:
                            client_socket.sendall(
                                (f"Last shared secret (hex): {session.last_shared_hex}\n").encode('utf-8')
                            )
                    elif cmd == 'KDF':
                        if session.last_shared_kdf is None:
                            client_socket.sendall(b"No shared secret yet.\n")
                        else:
                            client_socket.sendall(
                                (f"SHA256(last shared secret): {session.last_shared_kdf}\n").encode('utf-8')
                            )
                    elif cmd == 'RESET':
                        session._generate_ephemeral()
                        client_socket.sendall(b"Server private/public regenerated.\n")
                    elif cmd == 'STATUS':
                        msg = (
                            f"Current params: {session.params.name} ({session.params.size_bits()} bits)\n"
                            f"Server public: {session.public_key}\n"
                        )
                        if session.last_shared_hex:
                            msg += (
                                f"Last server public: {session.last_server_public}\n"
                                f"Last secret hex: {session.last_shared_hex}\n"
                                f"Last secret SHA256: {session.last_shared_kdf}\n"
                            )
                        client_socket.sendall(msg.encode('utf-8'))
                    else:
                        client_socket.sendall(b"Unknown command. Type HELP.\n")
                except Exception as exc:
                    client_socket.sendall((f"Internal error: {exc}\n").encode('utf-8'))
        finally:
            try:
                client_socket.close()
            except Exception:
                pass


if __name__ == '__main__':
    DiffieHellmanServer().start()
