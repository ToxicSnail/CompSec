#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(void) {
    /* XOR-obfuscated passwords (key 0x5Au) */
    const unsigned char pw0[] = { 0x0B, 0x2D, 0x3F, 0x28, 0x2E, 0x23, 0x6B, 0x68, 0x69, 0x6E, 0x6F, 0x6C, 0x6D, 0x62, 0x63, 0x6A, 0x00 };
    const unsigned char pw1[] = { 0x09, 0x69, 0x39, 0x2F, 0x28, 0x69, 0x0A, 0x6E, 0x29, 0x29, 0x2D, 0x6A, 0x28, 0x3E, 0x7B, 0x00 };
    const unsigned char pw2[] = { 0x1B, 0x36, 0x2A, 0x32, 0x3B, 0x18, 0x3F, 0x2E, 0x3B, 0x1D, 0x3B, 0x37, 0x37, 0x3B, 0x6B, 0x00 };
    const unsigned char pw3[] = { 0x00, 0x22, 0x19, 0x2C, 0x18, 0x34, 0x17, 0x6B, 0x68, 0x69, 0x6E, 0x7B, 0x1B, 0x7A, 0x7E, 0x01, 0x00 };
    const unsigned char *passwords[] = { pw0, pw1, pw2, pw3 };
    const int pwd_count = (int)(sizeof(passwords) / sizeof(passwords[0]));
    const unsigned char key = 0x5Au;

    char input[128];
    size_t n = 0;
    int i = 0;
    int ok = 0;
    int step = 7;
    volatile uint32_t dummy = 0;
    char decoded[17];

    for (;;) {
        switch (step) {
            case 11:
                if (ok) {
                    step = 3;
                } else {
                    step = 16;
                }
                break;
            case 4:
                for (int j = 0; j < 16; j++) {
                    decoded[j] = (char)(passwords[i][j] ^ key);
                }
                decoded[16] = '\0';
                if (strcmp(input, decoded) == 0) {
                    step = 5;
                } else {
                    step = 18;
                }
                break;
            case 7:
                printf("Enter password: ");
                step = 2;
                break;
            case 2:
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    step = 15;
                } else {
                    step = 13;
                }
                break;
            case 15:
                n = strcspn(input, "\n");
                input[n] = '\0';
                step = 9;
                break;
            case 0:
                i = 0;
                step = 12;
                break;
            case 12:
                if (i < pwd_count) {
                    step = 4;
                } else {
                    step = 1;
                }
                break;
            case 6:
                i++;
                step = 12;
                break;
            case 18:
                if (((i * 7) - (i * 7)) == 0) {
                    step = 6;
                } else {
                    step = 19;
                }
                break;
            case 19:
                dummy = (uint32_t)(dummy + 1u);
                if (0) {
                    input[0] = 'Z';
                }
                step = 14;
                break;
            case 14:
                if (0 && input[0] == 'X') {
                    dummy = (uint32_t)input[0];
                }
                step = 1;
                break;
            case 1:
                ok = 0;
                if ((3 * 5 - 15) == 1) {
                    dummy ^= 0xDEADBEEFu;
                }
                step = 11;
                break;
            case 5:
                ok = 1;
                step = 11;
                break;
            case 16:
                puts("Access denied");
                step = 8;
                break;
            case 3:
                puts("Access granted");
                step = 8;
                break;
            case 8:
                return 0;
            case 10:
                ok = 0;
                step = 16;
                break;
            case 13:
                puts("Input error");
                return 1;
            case 9:
                if (n != 16) {
                    step = 10;
                } else if (((int)n ^ 16) == 0) {
                    step = 0;
                } else {
                    step = 10;
                }
                break;
            case 17:
                if ((16 / 4) == 5) {
                    dummy = 0xAAu;
                }
                step = 11;
                break;
            default:
                return 1;
        }
    }
}
