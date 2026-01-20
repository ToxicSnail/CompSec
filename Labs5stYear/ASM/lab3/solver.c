#include <stdio.h>
#include <stdint.h>

int main(void) {
    const uint32_t seed1[] = { 0xABAABAABu, 0x2C9A7F15u, 0x8B31C6D2u, 0x45D9F3Bu };
    const uint32_t seed2[] = { 0xABAABABAu, 0x2C9A7F51u, 0x8B31C62Du, 0x45D9FB3u };
    const char alphabet[] = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    const int pwd_count = (int)(sizeof(seed1) / sizeof(seed1[0]));
    const uint32_t dummy = 0x13579BDFu;

    for (int i = 0; i < pwd_count; i++) {
        uint32_t s = seed1[i] ^ dummy;
        uint32_t k = seed2[i] * 1;
        char out[17];
        for (int j = 0; j < 16; j++) {
            s = s * 52525252u + k + (uint32_t)(j * 67u);
            out[j] = alphabet[(s >> 16) % (sizeof(alphabet) - 1)];
        }
        out[16] = '\0';
        puts(out);
    }

    return 0;
}
