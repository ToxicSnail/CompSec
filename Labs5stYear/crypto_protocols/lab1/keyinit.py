import json
import secrets
import sys
import os
from otp import compute_server_stored, to_hex64


USERS_FILE = os.path.join(os.path.dirname(__file__), 'skey_users.json')


def keyinit(username: str, secret_phrase: str, iterations: int = 100, seed: str | None = None):
    os.makedirs(os.path.dirname(USERS_FILE), exist_ok=True)
    try:
        with open(USERS_FILE, 'r') as f:
            users = json.load(f)
    except FileNotFoundError:
        users = {}

    if not seed:
        seed = secrets.token_hex(2).upper()

    last_value = compute_server_stored(seed, secret_phrase, iterations)
    users[username] = {
        'last_hash': to_hex64(last_value),
        'seed': seed,
        'seq': iterations,
        'max_seq': iterations,
    }

    with open(USERS_FILE, 'w') as f:
        json.dump(users, f, indent=2)

    print(f"User {username} initialized")
    print(f"Seed: {seed}")
    print(f"Iterations: {iterations}")
    print(f"Stored last_hash: {to_hex64(last_value)}")


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: python keyinit.py <username> <secret_phrase> [iterations] [seed]')
        sys.exit(1)

    username = sys.argv[1]
    secret_phrase = sys.argv[2]
    iterations = int(sys.argv[3]) if len(sys.argv) > 3 else 100
    seed = sys.argv[4] if len(sys.argv) > 4 else None

    keyinit(username, secret_phrase, iterations, seed)

