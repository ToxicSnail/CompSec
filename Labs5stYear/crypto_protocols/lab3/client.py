import argparse
import os
import socket
import sys

# Allow running as `python lab3/client.py`
sys.path.insert(0, os.path.dirname(__file__))

from dh import (
    DomainParameters,
    compute_kdf_sha256,
    compute_public_key,
    derive_shared_secret,
    generate_private_key,
    int_to_hex,
    parse_int,
    validate_parameters,
    validate_public_key,
)


PROMPT = b"> "


def read_until_prompt(sock: socket.socket) -> str:
    buffer = bytearray()
    while True:
        chunk = sock.recv(4096)
        if not chunk:
            break
        buffer.extend(chunk)
        if buffer.endswith(PROMPT):
            buffer = buffer[:-len(PROMPT)]
            break
    return buffer.decode('utf-8', errors='ignore')


def send_command(sock: socket.socket, command: str) -> str:
    sock.sendall((command.strip() + "\n").encode('utf-8'))
    return read_until_prompt(sock)


def parse_params_json(text: str) -> tuple[DomainParameters, int]:
    for line in text.splitlines():
        line = line.strip()
        if line.startswith('{') and line.endswith('}'):
            data_line = line
            break
    else:
        raise ValueError("Server did not return JSON payload for parameters")

    import json

    payload = json.loads(data_line)
    params = DomainParameters(
        p=int(payload['p']),
        g=int(payload['g']),
        q=int(payload['q']),
        name=payload.get('name', 'server-params'),
    )
    server_public = int(payload['server_public'])
    return params, server_public


def main():
    parser = argparse.ArgumentParser(description='Diffie-Hellman (RFC 2631) client for lab3')
    parser.add_argument('--host', default='localhost', help='Server host (default: localhost)')
    parser.add_argument('--port', type=int, default=12345, help='Server port (default: 12345)')
    parser.add_argument('--group', choices=['group5', 'group14', 'group15'], help='Request RFC 3526 group from server')
    parser.add_argument('--generate', type=int, help='Ask server to generate custom safe prime of given bit length')
    parser.add_argument('--private', type=str, help='Hex/decimal private exponent override (for reproducibility)')
    parser.add_argument('--verbose', action='store_true', help='Show server dialogue')

    args = parser.parse_args()

    with socket.create_connection((args.host, args.port)) as sock:
        banner = read_until_prompt(sock)
        if args.verbose:
            print(banner)

        if args.group:
            resp = send_command(sock, f"RFC {args.group}")
            if args.verbose:
                print(resp)

        if args.generate:
            resp = send_command(sock, f"GENERATE {args.generate}")
            if args.verbose:
                print(resp)

        params_resp = send_command(sock, "PARAMS JSON")
        if args.verbose:
            print(params_resp)
        params, server_public = parse_params_json(params_resp)

        if not validate_parameters(params):
            raise SystemExit("Server returned invalid parameters")
        if not validate_public_key(server_public, params):
            raise SystemExit("Server public key failed validation")

        if args.private:
            private_value = parse_int(args.private)
            if not (1 < private_value < params.q):
                raise SystemExit("Provided private exponent not in range (1, q)")
        else:
            private_value = generate_private_key(params.q)

        client_public = compute_public_key(params.g, private_value, params.p)
        if not validate_public_key(client_public, params):
            raise SystemExit("Derived client public key failed validation")

        resp = send_command(sock, f"PUB {client_public}")
        if args.verbose:
            print(resp)

        secret_hex = None
        sha256_remote = None
        for line in resp.splitlines():
            if line.startswith('SHARED_SECRET_HEX:'):
                secret_hex = line.split(':', 1)[1].strip()
            elif line.startswith('SHA256:'):
                sha256_remote = line.split(':', 1)[1].strip()
        if secret_hex is None:
            raise SystemExit("Server response lacked shared secret")

        peer_public = server_public
        local_secret = derive_shared_secret(peer_public, private_value, params)
        local_hex = int_to_hex(local_secret)
        local_sha256 = compute_kdf_sha256(local_secret)

        print("=== Diffie-Hellman key exchange ===")
        print(f"Parameters: {params.name} ({params.size_bits()} bits)")
        print(f"Server public (Yb): {server_public}")
        print(f"Client public (Ya): {client_public}")
        print(f"Derived secret (client): {local_hex}")
        if sha256_remote:
            print(f"Server SHA256(ZZ): {sha256_remote}")
        print(f"Client SHA256(ZZ): {local_sha256}")

        if sha256_remote and sha256_remote.upper() == local_sha256.upper():
            print("Shared secret match confirmed.")
        else:
            print("WARNING: Hash mismatch between client and server.")


if __name__ == '__main__':
    main()
