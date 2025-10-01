import argparse
import os
import sys

# Чтобы локальные импорты работали при запуске через `python lab2/client.py`
sys.path.insert(0, os.path.dirname(__file__))
from chap import compute_client_response


def main():
    parser = argparse.ArgumentParser(description='CHAP (RFC 1994) Client - compute MD5 response')
    parser.add_argument('--secret', type=str, help='Your shared secret')
    parser.add_argument('--id', type=int, help='Identifier (0..255)')
    parser.add_argument('--challenge', type=str, help='Challenge hex string')
    parser.add_argument('--interactive', action='store_true', help='Interactive mode')

    args = parser.parse_args()

    if args.interactive:
        print('=== CHAP-MD5 Client ===')
        secret = input('Enter your secret: ')
        ident = int(input('Enter identifier (0..255): '))
        challenge = input('Enter challenge (hex): ')
    else:
        if args.secret is None or args.id is None or args.challenge is None:
            parser.error('--secret, --id and --challenge are required unless --interactive is used')
        secret, ident, challenge = args.secret, int(args.id), args.challenge

    resp_hex = compute_client_response(ident, secret, challenge)
    print('Response (hex):')
    print(resp_hex)


if __name__ == '__main__':
    main()
