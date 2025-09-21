import argparse
from otp import compute_client_otp, to_words


def main():
    parser = argparse.ArgumentParser(description='S/KEY (RFC 1760) Client - compute one-time password')
    parser.add_argument('--secret', type=str, help='Your secret phrase')
    parser.add_argument('--seed', type=str, help='Seed from server challenge')
    parser.add_argument('--seq', type=int, help='Sequence number from challenge (n)')
    # Вывод ограничен только 6 словами
    parser.add_argument('--interactive', action='store_true', help='Interactive mode')

    args = parser.parse_args()

    if args.interactive:
        print('=== S/KEY One-time Password (RFC 1760) ===')
        secret = input('Enter your secret phrase: ')
        seed = input('Enter seed from server: ')
        seq = int(input('Enter sequence number (n): '))
        # Вывод всегда в виде 6 слов
    else:
        if not (args.secret and args.seed and args.seq is not None):
            parser.error('--secret, --seed and --seq are required unless --interactive is used')
        secret, seed, seq = args.secret, args.seed, args.seq

    # Клиент отправляет v_{n-1} (на один шаг меньше n)
    value = compute_client_otp(seed, secret, int(seq))
    out = to_words(value)

    print('One-time password:')
    print(out)


if __name__ == '__main__':
    main()
