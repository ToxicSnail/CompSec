#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(void) {
    const uint32_t seed1[] = { 0xABAABAABu, 0x2C9A7F15u, 0x8B31C6D2u, 0x45D9F3Bu };
    const uint32_t seed2[] = { 0xABAABABAu, 0x2C9A7F51u, 0x8B31C62Du, 0x45D9FB3u };
    const char alphabet[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    const int real_count = (int)(sizeof(seed1) / sizeof(seed1[0]));
    const int count = real_count + 3;

    char input[128];
    size_t n = 0;
    int i = 0;
    int ok = 0;
    int step = 7;
    uint32_t dummy = 0;
    uint32_t const_b = 0x13579ADFu;
    uint32_t dumpy = 0;
    char decoded[17];
    uint32_t t;

    for (;;) {
        switch (step) 
        {
            case 11:
                if (ok) {
                    step = 3;
                } else {
                    step = 16;
                }
                break;
            case 4:
                {
                    int idx = i % real_count;
                    uint32_t s = seed1[idx] ^ dummy;
                    uint32_t k = seed2[idx] * 1;
                    for (int j = 0; j < 16; j++) {
                        s = s * 52525252u + k + (uint32_t)(j * 67u);
                        decoded[j] = alphabet[(s >> 16) % (sizeof(alphabet) - 1)];
                    }
                    decoded[16] = '\0';
                    step = 21;
                }
                break;
            case 21:
            {
                if (strcmp(input, decoded) == 0 && i < real_count) {
                    step = 5;
                } else {
                    step = 18;
                }
                break;
            }
            case 7:
                printf("Enter 16-char password: ");
                dummy = 0x13579BDFu;
                t = 0x133337u;
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
                dumpy = dummy;
                break;
            case 0:
                i = 0;
                dummy = 0x13579BDFu ^ const_b;
                t ^= dummy;
                step = 12;
                break;
            case 12:
                if (i < count) {
                    dummy = dumpy;
                    step = 4;
                } else {
                    dummy ^= dumpy;
                    step = 1;
                }
                break;
            case 6:
                i++;
                step = 12;
                break;
            case 18:
                {
                    uint32_t p1 = t * t + 3u * t + 2u;
                    uint32_t p2 = (t + 1u) * (t + 2u);
                    if ((p1 - p2) == 0u) {
                        step = 6;
                    } else {
                        step = 19;
                    }
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
