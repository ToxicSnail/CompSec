import json
import sys
import os


USERS_FILE = os.path.join(os.path.dirname(__file__), 'chap_users.json')


def keyinit(username: str, secret: str):
    os.makedirs(os.path.dirname(USERS_FILE), exist_ok=True)
    try:
        with open(USERS_FILE, 'r') as f:
            users = json.load(f)
    except FileNotFoundError:
        users = {}

    users[username] = {
        'secret': secret
    }

    with open(USERS_FILE, 'w') as f:
        json.dump(users, f, indent=2)

    print(f"User {username} initialized")
    print("Warning: secret stored in plaintext (for lab only)")


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: python lab2/keyinit.py <username> <secret>')
        sys.exit(1)

    username = sys.argv[1]
    secret = sys.argv[2]
    keyinit(username, secret)

