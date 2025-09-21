import socket
import threading
import json
import os
import secrets
import time
from otp import f, from_words, to_hex64


USERS_FILE = os.path.join(os.path.dirname(__file__), 'skey_users.json')

 
def load_users():
    if os.path.exists(USERS_FILE):
        with open(USERS_FILE, 'r') as fjson:
            try:
                data = json.load(fjson)
            except Exception:
                return {}
        # Мигрируем старые ключи, если они присутствуют
        changed = False
        for u, rec in list(data.items()):
            if 'iterations' in rec or 'current_iteration' in rec:
                rec['max_seq'] = rec.get('iterations', rec.get('max_seq', 0))
                rec['seq'] = rec.get('current_iteration', rec.get('seq', rec['max_seq']))
                rec.pop('iterations', None)
                rec.pop('current_iteration', None)
                changed = True
        if changed:
            save_users(data)
        return data
    return {}


def save_users(users):
    os.makedirs(os.path.dirname(USERS_FILE), exist_ok=True)
    with open(USERS_FILE, 'w') as fjson:
        json.dump(users, fjson, indent=2)


class SKeyServer:
    def __init__(self, host='localhost', port=12345):
        self.host = host
        self.port = port
        self.users = load_users()
        # Ожидающие регистрации: reg_id -> {username, seed, n, created_at}
        self.pending = {}
        self.pending_lock = threading.Lock()
        # Параметры сервера
        self.DEFAULT_SEQ = 10
        self.PENDING_MAX = 10
        self.REG_TTL = 300  # секунды (5 минут)
        self._running = False

    def generate_seed(self) -> str:
        # 4 случайных байта -> 8 шестнадцатеричных символов (верхний регистр)
        return secrets.token_hex(2).upper()

    def generate_reg_id(self) -> str:
        return secrets.token_hex(8).upper()

    def cleanup_pending(self, verbose: bool = True):
        now = time.time()
        removed = []
        with self.pending_lock:
            keys = list(self.pending.keys())
            for rid in keys:
                rec = self.pending.get(rid)
                if not rec:
                    continue
                if now - rec['created_at'] > self.REG_TTL:
                    removed.append((rid, rec.get('username', '?')))
                    self.pending.pop(rid, None)
            count = len(self.pending)
        if verbose:
            print(f"[pending] count={count}")
            for rid, user in removed:
                print(f"[pending] expired: reg_id={rid} user={user}")

    def get_user_status(self, username: str):
        if username not in self.users:
            return None
        u = self.users[username]
        return {
            'remaining': u['seq'],
            'total': u['max_seq'],
            'seed': u['seed']
        }

    def generate_challenge(self, username: str):
        if username not in self.users:
            return None
        u = self.users[username]
        return f"{u['seq']} {u['seed']}"

    def verify_password(self, username: str, password_input: str) -> bool:
        if username not in self.users:
            return False
        u = self.users[username]

        try:
            value = from_words(password_input)
        except Exception:
            return False

        # Применяем одну итерацию и сравниваем с сохранённым last_hash
        test = f(value)
        try:
            stored = bytes.fromhex(u['last_hash'])
        except Exception:
            return False

        if test == stored:
            # Обновление: сохраняем полученное значение как новый last_hash
            u['last_hash'] = to_hex64(value)
            u['seq'] = max(0, int(u['seq']) - 1)
            save_users(self.users)
            return True
        return False

    def handle_client(self, client_socket):
        try:
            client_socket.sendall(
                (
                    "S/KEY Authentication Server (RFC 1760)\n"
                    "Commands:\n"
                    "  INIT <username>          - Start registration (no password)\n"
                    "  INIT_FINAL <reg_id> <n> <v_n_words> - Finish registration (6 words only)\n"
                    "  AUTH <user> <6words>     - Authenticate (6 words only)\n"
                    "  STATUS <username>        - Show status and seed\n"
                    "  LIST                     - List users\n"
                    "  QUIT                     - Disconnect\n\n"
                ).encode('utf-8')
            )

            while True:
                client_socket.sendall(b"> ")
                data = client_socket.recv(4096)
                if not data:
                    break
                request = data.decode('utf-8', errors='ignore').strip()
                if not request:
                    continue
                if request.upper() == 'QUIT':
                    break

                parts = request.split()
                cmd = parts[0].upper()

                if cmd == 'HELP':
                    client_socket.sendall(b"Type commands shown above.\n")

                elif cmd == 'LIST':
                    if not self.users:
                        client_socket.sendall(b"No users.\n")
                    else:
                        lines = [f"  {u} - {self.users[u]['seq']} left" for u in self.users]
                        client_socket.sendall(("\n".join(lines) + "\n").encode('utf-8'))

                elif cmd == 'STATUS' and len(parts) == 2:
                    username = parts[1]
                    st = self.get_user_status(username)
                    if not st:
                        client_socket.sendall(b"User not found\n")
                    else:
                        msg = (
                            f"User: {username}\n"
                            f"Seed: {st['seed']}\n"
                            f"Remaining: {st['remaining']}/{st['total']}\n"
                            f"Challenge: {st['remaining']} {st['seed']}\n"
                            f"Use client.py with --seq {st['remaining']} (client computes n-1).\n"
                        )
                        client_socket.sendall(msg.encode('utf-8'))

                elif cmd == 'INIT' and len(parts) == 2:
                    username = parts[1]
                    if username in self.users:
                        client_socket.sendall(b"User already exists\n")
                        continue
                    # Создаём запись ожидающей регистрации
                    self.cleanup_pending(verbose=False)
                    with self.pending_lock:
                        if len(self.pending) >= self.PENDING_MAX:
                            client_socket.sendall(b"Too many registrations in progress, try later\n")
                            continue
                        reg_id = self.generate_reg_id()
                        seed = self.generate_seed()
                        n = self.DEFAULT_SEQ
                        self.pending[reg_id] = {
                            'username': username,
                            'seed': seed,
                            'n': n,
                            'created_at': time.time(),
                        }
                    msg = (
                        f"REG_ID: {reg_id}\n"
                        f"SEED:   {seed}\n"
                        f"N:      {n}\n\n"
                        f"Compute v_n locally (do not send secret) and finish with:\n"
                        f"  INIT_FINAL {reg_id} {n} <v_n_as_6_words>\n"
                    )
                    client_socket.sendall(msg.encode('utf-8'))

                elif cmd == 'INIT_FINAL' and len(parts) >= 4:
                    reg_id = parts[1]
                    try:
                        n = int(parts[2])
                    except ValueError:
                        client_socket.sendall(b"Invalid n\n")
                        continue
                    password_input = ' '.join(parts[3:])

                    self.cleanup_pending(verbose=False)
                    with self.pending_lock:
                        rec = self.pending.get(reg_id)
                    if not rec:
                        client_socket.sendall(b"Invalid or expired reg_id\n")
                        continue
                    if rec['n'] != n:
                        client_socket.sendall(b"N mismatch\n")
                        continue
                    username = rec['username']
                    if username in self.users:
                        client_socket.sendall(b"User already exists\n")
                        with self.pending_lock:
                            self.pending.pop(reg_id, None)
                        continue
                    try:
                        v_n = from_words(password_input)
                    except Exception as e:
                        client_socket.sendall(f"Invalid password: {e}\n".encode('utf-8'))
                        continue
                    # Store user
                    self.users[username] = {
                        'last_hash': to_hex64(v_n),
                        'seed': rec['seed'],
                        'seq': n,
                        'max_seq': n,
                    }
                    save_users(self.users)
                    with self.pending_lock:
                        self.pending.pop(reg_id, None)
                    client_socket.sendall((f"Registered OK. Challenge: {n} {rec['seed']}\n").encode('utf-8'))

                elif cmd == 'AUTH' and len(parts) >= 3:
                    username = parts[1]
                    password_input = ' '.join(parts[2:])
                    if self.verify_password(username, password_input):
                        st = self.get_user_status(username)
                        msg = (
                            f"OK. Remaining: {st['remaining']}\n"
                            f"Next challenge: {st['remaining']} {st['seed']}\n"
                        )
                        client_socket.sendall(msg.encode('utf-8'))
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
            time.sleep(60)

    def start(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((self.host, self.port))
        sock.listen(5)
        print(f"S/KEY server on {self.host}:{self.port}; users file: {USERS_FILE}")
        print(f"Default N: {self.DEFAULT_SEQ}; pending TTL: {self.REG_TTL}s; max pending: {self.PENDING_MAX}")
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
    SKeyServer().start()
