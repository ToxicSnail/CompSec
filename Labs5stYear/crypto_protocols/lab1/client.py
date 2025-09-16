import argparse
from otp import compute_client_otp, to_words


def main():
    parser = argparse.ArgumentParser(description='S/KEY (RFC 1760) Client - compute one-time password')
    parser.add_argument('--secret', type=str, help='Your secret phrase')
    parser.add_argument('--seed', type=str, help='Seed from server challenge')
    parser.add_argument('--seq', type=int, help='Sequence number from challenge (n)')
    # Output restricted to 6 words only
    parser.add_argument('--interactive', action='store_true', help='Interactive mode')

    args = parser.parse_args()

    if args.interactive:
        print('=== S/KEY One-time Password (RFC 1760) ===')
        secret = input('Enter your secret phrase: ')
        seed = input('Enter seed from server: ')
        seq = int(input('Enter sequence number (n): '))
        # Output is always words
    else:
        if not (args.secret and args.seed and args.seq is not None):
            parser.error('--secret, --seed and --seq are required unless --interactive is used')
        secret, seed, seq = args.secret, args.seed, args.seq

    # Client responds with v_{n-1}
    value = compute_client_otp(seed, secret, int(seq))
    out = to_words(value)

    print('One-time password:')
    print(out)


if __name__ == '__main__':
    main()
