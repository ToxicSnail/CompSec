import socket
import threading
import json
import os
import secrets
import time
import sys

# Чтобы локальные импорты работали при запуске через `python lab2/server.py`
sys.path.insert(0, os.path.dirname(__file__))
from chap import md5_chap_response, to_hex, from_hex, generate_challenge

USERS_FILE = os.path.join(os.path.dirname(__file__), 'chap_users.json')

def load_users():
    if os.path.exists(USERS_FILE):
        try:
            with open(USERS_FILE, 'r') as fjson:
                return json.load(fjson)
        except Exception:
            return {}
    return {}

def save_users(users):
    os.makedirs(os.path.dirname(USERS_FILE), exist_ok=True)
    with open(USERS_FILE, 'w') as fjson:
        json.dump(users, fjson, indent=2)

class ChapServer:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port
        self.users = load_users()  # структура: { имя: {"secret": "..."} }
        # pending[(имя, идентификатор)] = { 'challenge': bytes, 'ts': float }
        self.pending = {}
        self.pending_lock = threading.Lock()
        self.DEFAULT_CHL_LEN = 16
        self.REG_TTL = 120  # секунды
        self.MAX_PENDING = 100
        self._running = False

    def has_user(self, username: str) -> bool:
        return username in self.users

    def add_user(self, username: str, secret: str):
        self.users[username] = { 'secret': secret }
        save_users(self.users)

    def cleanup_pending(self, verbose: bool = True):
        now = time.time()
        removed = []
        with self.pending_lock:
            keys = list(self.pending.keys())
            for k in keys:
                rec = self.pending.get(k)
                if not rec:
                    continue
                if now - rec['ts'] > self.REG_TTL:
                    removed.append(k)
                    self.pending.pop(k, None)
        if verbose:
            print(f"[pending] active={len(self.pending)} removed={len(removed)}")

    def create_challenge(self, username: str, length: int | None = None):
        if not self.has_user(username):
            return None
        ch_len = int(length or self.DEFAULT_CHL_LEN)
        challenge = generate_challenge(ch_len)
        identifier = secrets.randbelow(256)
        with self.pending_lock:
            if len(self.pending) >= self.MAX_PENDING:
                # удаляем самый старый челлендж
                oldest = sorted(self.pending.items(), key=lambda kv: kv[1]['ts'])[0][0]
                self.pending.pop(oldest, None)
            self.pending[(username, identifier)] = { 'challenge': challenge, 'ts': time.time() }
        return identifier, challenge

    def verify(self, username: str, identifier: int, response_hex: str) -> bool:
        key = (username, identifier)
        with self.pending_lock:
            rec = self.pending.get(key)
        if not rec:
            return False
        try:
            response = from_hex(response_hex)
        except Exception:
            return False
        if len(response) != 16:
            return False
        secret = self.users.get(username, {}).get('secret')
        if secret is None:
            return False
        expected = md5_chap_response(identifier, secret, rec['challenge'])
        ok = expected == response
        # одноразовая проверка для выданного челленджа
        with self.pending_lock:
            self.pending.pop(key, None)
        return ok

    def handle_client(self, client_socket):
        try:
            client_socket.sendall((
                "CHAP-MD5 Authentication Server (RFC 1994)\n"
                "Commands:\n"
                "  LIST                           - List users\n"
                "  STATUS <username>              - Check user presence\n"
                "  CHALLENGE <username> [len]     - Issue challenge (len=4..255, default 16)\n"
                "  AUTH <username> <id> <hex>     - Verify response (hex=MD5)\n"
                "  HELP                           - This help\n"
                "  QUIT                           - Close\n"
            ).encode('utf-8'))
            while True:
                client_socket.sendall(b"> ")
                data = client_socket.recv(4096)
                if not data:
                    break
                req = data.decode('utf-8', errors='ignore').strip()
                if not req:
                    continue
                if req.upper() == 'QUIT':
                    break

                parts = req.split()
                cmd = parts[0].upper()

                if cmd == 'HELP':
                    client_socket.sendall(b"Type commands shown above.\n")

                elif cmd == 'LIST':
                    if not self.users:
                        client_socket.sendall(b"No users.\n")
                    else:
                        lines = [f"  {u}" for u in sorted(self.users.keys())]
                        client_socket.sendall(("\n".join(lines) + "\n").encode('utf-8'))

                elif cmd == 'STATUS' and len(parts) == 2:
                    username = parts[1]
                    if not self.has_user(username):
                        client_socket.sendall(b"User not found\n")
                    else:
                        client_socket.sendall(b"User exists\n")

                elif cmd == 'CHALLENGE' and len(parts) in (2, 3):
                    username = parts[1]
                    length = None
                    if len(parts) == 3:
                        try:
                            length = int(parts[2])
                        except ValueError:
                            client_socket.sendall(b"Invalid length\n")
                            continue
                    res = self.create_challenge(username, length)
                    if not res:
                        client_socket.sendall(b"User not found\n")
                        continue
                    ident, challenge = res
                    msg = (
                        f"ID: {ident}\n"
                        f"CHALLENGE: {to_hex(challenge)}\n"
                        f"ALGO: MD5\n"
                    )
                    client_socket.sendall(msg.encode('utf-8'))

                elif cmd == 'AUTH' and len(parts) == 4:
                    username = parts[1]
                    try:
                        ident = int(parts[2])
                    except ValueError:
                        client_socket.sendall(b"Invalid id\n")
                        continue
                    resp_hex = parts[3]
                    ok = self.verify(username, ident, resp_hex)
                    if ok:
                        client_socket.sendall(b"OK\n")
                    else:
                        client_socket.sendall(b"FAIL\n")

                else:
                    client_socket.sendall(b"Invalid command. Type HELP.\n")
        finally:
            try:
                client_socket.close()
            except Exception:
                pass

    def _pending_cleaner(self):
        while self._running:
            try:
                self.cleanup_pending(verbose=True)
            except Exception as e:
                print(f"[pending] cleanup error: {e}")
            time.sleep(30)

    def start(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((self.host, self.port))
        sock.listen(5)
        print(f"CHAP-MD5 server on {self.host}:{self.port}; users file: {USERS_FILE}")
        print(f"Challenge default length: {self.DEFAULT_CHL_LEN}; pending TTL: {self.REG_TTL}s; max pending: {self.MAX_PENDING}")
        self._running = True
        cleaner = threading.Thread(target=self._pending_cleaner, daemon=True)
        cleaner.start()
        try:
            while True:
                client, addr = sock.accept()
                print("Connection from", addr)
                t = threading.Thread(target=self.handle_client, args=(client,))
                t.daemon = True
                t.start()
        except KeyboardInterrupt:
            print("Stopping...")
        finally:
            self._running = False
            sock.close()

if __name__ == '__main__':
    ChapServer().start()