import argparse
import json
import os
import socket
import sys
from typing import Dict, Tuple

sys.path.insert(0, os.path.dirname(__file__))

from protocol import canonical_json, hex_encode, sha256


def _send_json(sock: socket.socket, obj: Dict) -> None:
    data = json.dumps(obj, sort_keys=True, separators=(",", ":")).encode("utf-8") + b"\n"
    sock.sendall(data)


def _recv_json(sock: socket.socket, buffer: bytearray) -> Tuple[Dict, bytearray]:
    while True:
        i = buffer.find(b"\n")
        if i != -1:
            line = buffer[:i]
            del buffer[: i + 1]
            if not line:
                continue
            return json.loads(line.decode("utf-8")), buffer
        chunk = sock.recv(4096)
        if not chunk:
            raise ConnectionError("server closed")
        buffer.extend(chunk)


def connect(host: str, port: int) -> socket.socket:
    try:
        s = socket.create_connection((host, port), timeout=5)
        try:
            s.settimeout(0.2)
            banner = s.recv(4096)
            if banner:
                print(banner.decode("utf-8", errors="ignore").strip())
        except Exception:
            pass
        finally:
            s.settimeout(None)
        return s
    except OSError as exc:
        print(f"Unable to connect: {exc}")
        sys.exit(1)


def cmd_stamp(args: argparse.Namespace) -> None:
    if not args.text and not args.file:
        print("Provide --text or --file")
        sys.exit(2)
    data = args.text.encode("utf-8") if args.text else open(args.file, "rb").read()
    h = hex_encode(sha256(data))
    s = connect(args.host, args.port)
    try:
        _send_json(s, {"action": "STAMP", "name": args.name, "doc_hash": h})
        buffer = bytearray()
        msg, _ = _recv_json(s, buffer)
        if msg.get("type") != "STAMPED":
            print("Unexpected response:", msg)
            sys.exit(1)
        print("Token issued (save this JSON to verify later):")
        print(json.dumps(msg, indent=2, sort_keys=True))
    finally:
        s.close()


def cmd_verify(args: argparse.Namespace) -> None:
    token_msg = json.load(open(args.token, "r", encoding="utf-8"))
    s = connect(args.host, args.port)
    try:
        _send_json(
            s,
            {
                "action": "VERIFY",
                "token": token_msg["token"],
                "signature": token_msg["signature"],
                "pubkey": token_msg.get("pubkey"),
            },
        )
        buffer = bytearray()
        msg, _ = _recv_json(s, buffer)
        print(json.dumps(msg, indent=2, sort_keys=True))
    finally:
        s.close()


def cmd_neighbor(args: argparse.Namespace) -> None:
    s = connect(args.host, args.port)
    try:
        _send_json(s, {"action": "NEIGHBOR", "n": args.n})
        buffer = bytearray()
        msg, _ = _recv_json(s, buffer)
        print(json.dumps(msg, indent=2, sort_keys=True))
    finally:
        s.close()


def cmd_get(args: argparse.Namespace) -> None:
    s = connect(args.host, args.port)
    try:
        _send_json(s, {"action": "GET", "n": args.n})
        buffer = bytearray()
        msg, _ = _recv_json(s, buffer)
        print(json.dumps(msg, indent=2, sort_keys=True))
    finally:
        s.close()


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Lab5 client (Alice)")
    p.add_argument("--host", default="127.0.0.1")
    p.add_argument("--port", type=int, default=13000)
    sub = p.add_subparsers(dest="cmd", required=True)

    s_stamp = sub.add_parser("stamp", help="Request timestamp for text or file")
    s_stamp.add_argument("--name", required=True, help="Alice's identifier")
    s_stamp.add_argument("--text", help="Inline text to hash")
    s_stamp.add_argument("--file", help="File path to hash")
    s_stamp.set_defaults(func=cmd_stamp)

    s_verify = sub.add_parser("verify", help="Verify a saved token JSON")
    s_verify.add_argument("--token", required=True, help="Path to JSON from STAMPED response")
    s_verify.set_defaults(func=cmd_verify)

    s_neighbor = sub.add_parser("neighbor", help="Get next requester after token n")
    s_neighbor.add_argument("--n", type=int, required=True)
    s_neighbor.set_defaults(func=cmd_neighbor)

    s_get = sub.add_parser("get", help="Fetch token by sequence number")
    s_get.add_argument("--n", type=int, required=True)
    s_get.set_defaults(func=cmd_get)

    return p.parse_args()


def main() -> None:
    args = parse_args()
    args.func(args)


if __name__ == "__main__":
    main()

