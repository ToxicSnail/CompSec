import argparse
import json
import os
import socket
import sys
import threading
import traceback
from typing import Tuple

# Allow running as `python lab4/server.py`
sys.path.insert(0, os.path.dirname(__file__))

from oakley import OakleyMainModeResponder


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Oakley Main Mode responder (lab4)")
    parser.add_argument("--host", default="127.0.0.1", help="Listen address (default: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=12000, help="Listen port (default: 12000)")
    parser.add_argument("--psk", default="lab4-shared-secret", help="Pre-shared key (utf-8 string)")
    parser.add_argument(
        "--identity",
        default="192.168.50.1",
        help="Responder ID payload value (IPv4 string for ID_IPV4_ADDR)",
    )
    parser.add_argument("--group", type=int, default=2, help="Oakley group id (default: 2)")
    parser.add_argument("--verbose", action="store_true", help="Print debug logs")
    return parser.parse_args()


def _recv_json(sock: socket.socket, buffer: bytearray) -> Tuple[dict, bytearray]:
    while True:
        newline_index = buffer.find(b"\n")
        if newline_index != -1:
            line = buffer[:newline_index].decode("utf-8").strip()
            del buffer[: newline_index + 1]
            if not line:
                continue
            try:
                return json.loads(line), buffer
            except json.JSONDecodeError as exc:
                raise ValueError(f"Invalid JSON from peer: {exc}") from exc
        chunk = sock.recv(4096)
        if not chunk:
            raise ConnectionError("Peer closed connection")
        buffer.extend(chunk)


def _send_json(sock: socket.socket, message: dict) -> None:
    data = json.dumps(message, sort_keys=True, separators=(",", ":")).encode("utf-8") + b"\n"
    sock.sendall(data)


def handle_client(client: socket.socket, addr: Tuple[str, int], args: argparse.Namespace) -> None:
    buffer = bytearray()
    try:
        client.sendall(
            (
                "Oakley Main Mode Responder (RFC 2412 example)\n"
                "Send four JSON messages (MSG1..MSG4) as described in README.\n"
                "This responder expects MSG1 first.\n"
            ).encode("utf-8")
        )
        responder = OakleyMainModeResponder(
            psk=args.psk.encode("utf-8"),
            identity=args.identity,
            group_id=args.group,
        )
        msg1, buffer = _recv_json(client, buffer)
        if args.verbose:
            print(f"[server] <- MSG1 from {addr}: {msg1}")
        msg2 = responder.process_message_one(msg1)
        _send_json(client, msg2)
        if args.verbose:
            print(f"[server] -> MSG2 to {addr}: {msg2}")
        msg3, buffer = _recv_json(client, buffer)
        if args.verbose:
            print(f"[server] <- MSG3 from {addr}: {msg3}")
        msg4 = responder.process_message_three(msg3)
        _send_json(client, msg4)
        if args.verbose:
            print(f"[server] -> MSG4 to {addr}: {msg4}")
        if responder.keys is not None:
            summary = {
                "type": "RESULT",
                "shared_secret": responder.shared_secret_hex,
                "keys": responder.keys.as_hex(),
                "note": "Summary payload for lab verification; not part of Oakley.",
            }
            _send_json(client, summary)
            if args.verbose:
                print(f"[server] -> RESULT to {addr}: {summary}")
    except Exception as exc:  # noqa: BLE001 - lab server should survive any exception
        error_msg = {"type": "ERROR", "error": str(exc)}
        try:
            _send_json(client, error_msg)
        except Exception:
            pass
        if args.verbose:
            traceback.print_exc()
    finally:
        try:
            client.close()
        except Exception:
            pass
        if args.verbose:
            print(f"[server] Connection with {addr} closed")


def main() -> None:
    args = parse_args()
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((args.host, args.port))
    sock.listen(5)
    print(f"Oakley responder listening on {args.host}:{args.port}, group {args.group}")
    print("Awaiting MSG1 from initiators...")
    try:
        while True:
            client, addr = sock.accept()
            print(f"Incoming connection from {addr}")
            thread = threading.Thread(target=handle_client, args=(client, addr, args), daemon=True)
            thread.start()
    except KeyboardInterrupt:
        print("Stopping Oakley responder...")
    finally:
        sock.close()


if __name__ == "__main__":
    main()
