import argparse
import json
import os
import socket
import sys
from typing import Tuple

# Allow running as `python lab4/client.py`
sys.path.insert(0, os.path.dirname(__file__))

from oakley import OakleyMainModeInitiator


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Oakley Main Mode initiator (lab4)")
    parser.add_argument("--host", default="127.0.0.1", help="Server host (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=12000, help="Server port (default: 12000)")
    parser.add_argument("--psk", default="lab4-shared-secret", help="Pre-shared key (utf-8 string)")
    parser.add_argument(
        "--identity",
        default="192.168.50.10",
        help="Initiator ID payload value (IPv4 string for ID_IPV4_ADDR)",
    )
    parser.add_argument("--group", type=int, default=2, help="Oakley group id (default: 2)")
    parser.add_argument("--verbose", action="store_true", help="Print verbose debug output")
    parser.add_argument("--dump", action="store_true", help="Dump raw JSON messages")
    return parser.parse_args()


def _recv_json(sock: socket.socket, buffer: bytearray) -> Tuple[dict, bytearray]:
    while True:
        newline_index = buffer.find(b"\n")
        if newline_index != -1:
            line = buffer[:newline_index].decode("utf-8").strip()
            del buffer[: newline_index + 1]
            if not line:
                continue
            return json.loads(line), buffer
        chunk = sock.recv(4096)
        if not chunk:
            raise ConnectionError("Connection closed by server")
        buffer.extend(chunk)


def _send_json(sock: socket.socket, message: dict) -> None:
    data = json.dumps(message, sort_keys=True, separators=(",", ":")).encode("utf-8") + b"\n"
    sock.sendall(data)


def main() -> None:
    args = parse_args()
    init = OakleyMainModeInitiator(
        psk=args.psk.encode("utf-8"),
        identity=args.identity,
        group_id=args.group,
    )
    try:
        sock = socket.create_connection((args.host, args.port), timeout=5)
    except OSError as exc:
        print(f"Unable to connect to {args.host}:{args.port}: {exc}")
        sys.exit(1)
    try:
        sock.settimeout(0.5)
        try:
            banner = sock.recv(4096)
            if banner:
                print(banner.decode("utf-8", errors="ignore").strip())
        except socket.timeout:
            pass
        finally:
            sock.settimeout(None)
        buffer = bytearray()
        try:
            msg1 = init.build_message_one()
            if args.verbose or args.dump:
                print("[client] -> MSG1", msg1)
            _send_json(sock, msg1)
            msg2, buffer = _recv_json(sock, buffer)
            if msg2.get("type") == "ERROR":
                raise RuntimeError(f"Server reported error: {msg2.get('error')}")
            if args.verbose or args.dump:
                print("[client] <- MSG2", msg2)
            msg3 = init.process_message_two(msg2)
            if args.verbose or args.dump:
                print("[client] -> MSG3", msg3)
            _send_json(sock, msg3)
            msg4, buffer = _recv_json(sock, buffer)
            if msg4.get("type") == "ERROR":
                raise RuntimeError(f"Server reported error: {msg4.get('error')}")
            if args.verbose or args.dump:
                print("[client] <- MSG4", msg4)
            summary = init.process_message_four(msg4)
            print("Handshake completed successfully.")
            print(f"Shared secret g^xy (hex): {init.shared_secret_hex}")
            print("Derived keys (HMAC-SHA1):")
            for key_name, key_value in init.keys.as_hex().items():
                print(f"  {key_name}: {key_value}")
            print("Responder identity payload:", summary.get("responder_id"))
            result = None
            try:
                sock.settimeout(0.2)
                result, buffer = _recv_json(sock, buffer)
            except (socket.timeout, ConnectionError):
                result = None
            finally:
                sock.settimeout(None)
            if isinstance(result, dict) and result.get("type") == "RESULT":
                print("Server summary:", result)
            elif result is not None and args.dump:
                print("Unexpected trailing message:", result)
        except Exception as exc:
            if args.verbose:
                raise
            print(f"Handshake failed: {exc}")
            sys.exit(1)
    finally:
        sock.close()


if __name__ == "__main__":
    main()
