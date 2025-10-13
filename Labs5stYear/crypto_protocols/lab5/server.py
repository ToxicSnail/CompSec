import argparse
import json
import os
import socket
import sys
import threading
import time
from typing import Dict, List, Optional, Tuple

sys.path.insert(0, os.path.dirname(__file__))

from protocol import (
    RSAPrivateKey,
    RSAPublicKey,
    Token,
    canonical_json,
    compute_link_hash,
    rsa_generate,
    rsa_sign,
    summarize_token,
)


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Lab5 Trent server — Linking Timestamp Protocol")
    p.add_argument("--host", default="127.0.0.1")
    p.add_argument("--port", type=int, default=13000)
    p.add_argument("--bits", type=int, default=1024, help="RSA key size (educational)")
    p.add_argument("--verbose", action="store_true")
    return p.parse_args()


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
            raise ConnectionError("peer closed")
        buffer.extend(chunk)


class TrentState:
    def __init__(self, pub: RSAPublicKey, priv: RSAPrivateKey):
        self.pub = pub
        self.priv = priv
        self.tokens: List[Token] = []
        self.next_requesters: Dict[int, str] = {}  # n -> next requester name

    def last_link(self) -> str:
        if not self.tokens:
            return "00" * 32
        return self.tokens[-1].link_hash

    def last_summary(self) -> Optional[Dict]:
        if not self.tokens:
            return None
        return summarize_token(self.tokens[-1])

    def create_token(self, name: str, doc_hash: str) -> Tuple[Token, int]:
        n = len(self.tokens) + 1
        ts = int(time.time())
        l_prev = self.last_link()
        link = compute_link_hash(n, name, doc_hash, ts, l_prev)
        tok = Token(
            n=n,
            time=ts,
            requester=name,
            doc_hash=doc_hash,
            prev_summary=self.last_summary(),
            link_hash=link,
        )
        self.tokens.append(tok)
        # record next requester for previous token if exists
        if tok.prev_summary is not None:
            self.next_requesters[tok.prev_summary["n"]] = name
        sig = rsa_sign(self.priv, tok.canonical())
        return tok, sig


def handle_client(sock: socket.socket, addr: Tuple[str, int], state: TrentState, verbose: bool) -> None:
    if verbose:
        print(f"[server] connection from {addr}")
    try:
        sock.sendall(
            (
                "Trent — Linking Timestamp Protocol (Lab5)\n"
                "Send JSON per line. Actions: STAMP, VERIFY, GET, NEIGHBOR, PUBKEY.\n"
            ).encode("utf-8")
        )
        buffer = bytearray()
        while True:
            try:
                msg, buffer = _recv_json(sock, buffer)
            except ConnectionError:
                break
            action = msg.get("action", "").upper()
            if action == "PUBKEY":
                _send_json(sock, {"type": "PUBKEY", "pubkey": state.pub.as_dict()})
                continue
            if action == "STAMP":
                name = msg.get("name")
                doc_hash = msg.get("doc_hash")
                if not isinstance(name, str) or not isinstance(doc_hash, str):
                    _send_json(sock, {"type": "ERROR", "error": "name and doc_hash required"})
                    continue
                tok, sig = state.create_token(name, doc_hash)
                resp = {
                    "type": "STAMPED",
                    "token": tok.body(),
                    "signature": str(sig),
                    "pubkey": state.pub.as_dict(),
                }
                _send_json(sock, resp)
                if verbose:
                    print(f"[server] STAMP n={tok.n} for {name} from {addr}")
                continue
            if action == "GET":
                n = int(msg.get("n", 0))
                if 1 <= n <= len(state.tokens):
                    _send_json(sock, {"type": "GOT", "token": state.tokens[n - 1].body()})
                else:
                    _send_json(sock, {"type": "ERROR", "error": "not found"})
                continue
            if action == "NEIGHBOR":
                n = int(msg.get("n", 0))
                res: Dict[str, Optional[str]] = {"type": "NEIGHBOR", "n": n}
                res["next_requester"] = state.next_requesters.get(n)
                _send_json(sock, res)
                continue
            if action == "VERIFY":
                token = msg.get("token")
                signature = msg.get("signature")
                pubkey = msg.get("pubkey") or state.pub.as_dict()
                try:
                    pub = RSAPublicKey(n=int(pubkey["n"]), e=int(pubkey["e"]))
                    sig_int = int(signature)
                    body_json = canonical_json(token)
                    valid = pow(sig_int, pub.e, pub.n) == int.from_bytes(
                        __import__("hashlib").sha256(body_json).digest(), "big"
                    ) % pub.n
                    # also check link consistency if we have this n
                    reason = None
                    if valid and 1 <= int(token.get("n", 0)) <= len(state.tokens):
                        stored = state.tokens[int(token["n"]) - 1]
                        if token.get("link_hash") != stored.link_hash:
                            valid = False
                            reason = "link_hash mismatch"
                    _send_json(sock, {"type": "VERIFIED", "valid": bool(valid), **({"reason": reason} if reason else {})})
                except Exception as exc:  # noqa: BLE001
                    _send_json(sock, {"type": "VERIFIED", "valid": False, "reason": str(exc)})
                continue
            _send_json(sock, {"type": "ERROR", "error": "unknown action"})
    finally:
        try:
            sock.close()
        except Exception:
            pass
        if verbose:
            print(f"[server] connection with {addr} closed")


def main() -> None:
    args = parse_args()
    pub, priv = rsa_generate(bits=args.bits)
    state = TrentState(pub, priv)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((args.host, args.port))
    s.listen(5)
    print(f"Trent server on {args.host}:{args.port} (RSA {args.bits} bits)")
    try:
        while True:
            c, a = s.accept()
            t = threading.Thread(target=handle_client, args=(c, a, state, args.verbose), daemon=True)
            t.start()
    except KeyboardInterrupt:
        print("Stopping Trent...")
    finally:
        s.close()


if __name__ == "__main__":
    main()

