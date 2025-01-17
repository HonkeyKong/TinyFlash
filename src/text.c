#include <text.h>

uint8_t currentLine = 0;

void DMACopy(void* dest, const void* src, uint32_t size) {
    REG_DMA3SAD = (uint32_t)src;          // Set source address
    REG_DMA3DAD = (uint32_t)dest;         // Set destination address
    REG_DMA3CNT_L = size / 2;             // Transfer size in 16-bit units
    REG_DMA3CNT_H = 0x8000 | 0x0000;      // Enable DMA, 16-bit mode, immediate transfer
}

// The lengths I go to in order to shave off a few bytes...

uint32_t stringLength(const char* str) {
    uint32_t length = 0;
    while (*str++) length++;
    return length;
}

void ClearText() {
    __asm__ volatile (
        "mov r0, %0\n"          // r0 = MAP_BASE_3 (start address)
        "mov r1, #0\n"          // r1 = 0 (value to write)
        "mov r2, #128\n"        // r2 = 128
        "lsl r2, r2, #2\n"      // r2 = 128 * 4 = 512 (iterations)
        "1:\n"                  // Loop label
        "str r1, [r0]\n"        // Store 0 at [r0]
        "add r0, r0, #4\n"      // Increment r0 by 4 manually
        "mov r3, r2\n"          // Move r2 into r3 (low register)
        "sub r3, r3, #1\n"      // Decrement r3
        "mov r2, r3\n"          // Move result back to r2
        "cmp r2, #0\n"          // Compare r2 with 0
        "bne 1b\n"              // If r2 != 0, repeat loop
        :
        : "r" (MAP_BASE_3)      // Input: MAP_BASE_3
        : "r0", "r1", "r2", "r3" // Clobbered registers
    );
}

void CheckLines() {
    if(currentLine >= 20) {
        ClearText();
        currentLine = 0;
    }
}

void RenderText(const char* text, uint8_t x, uint8_t y) {
    uint16_t* tilemap = (uint16_t*)MAP_BASE_3;
    int offset = y * 32 + x;

    for (int i = 0; text[i] != '\0'; ++i) {
        tilemap[offset + i] = text[i] - 32;
    }
}

void RenderLine(const char *text, uint8_t line) {
    CheckLines();
    RenderText(text, 0, line); // Render at x = 0, y = line
}

const char* IntToASCII(int value) {
    static char buffer[12]; // Max 10 digits + sign + null terminator
    int i = 10;             // Start at the end of the buffer
    buffer[11] = '\0';

    if (value == 0) return "0";

    while (value && i) {
        buffer[--i] = '0' + (value % 10);
        value /= 10;
    }

    return &buffer[i];
}

void RenderHex(unsigned int value, uint8_t x, uint8_t y) {
    char hexChars[] = "0123456789ABCDEF";
    for (int i = 7; i >= 0; --i) { // 8 hex digits for a 32-bit address
        char c = hexChars[(value >> (i * 4)) & 0xF];
        RenderText(&c, x++, y);
    }
}

// Returns a pointer to a static buffer containing the hex representation of `value`.
// Each call overwrites the same static buffer.
const char* HexString(uint16_t value)
{
    static char hexBuf[5]; // 4 hex digits + null terminator

    for (int i = 3; i >= 0; i--)
    {
        int nib = value & 0xF;
        hexBuf[i] = (nib < 10) ? (char)('0' + nib) : (char)('A' + (nib - 10));
        value >>= 4;
    }

    hexBuf[4] = '\0'; // Null-terminate
    return hexBuf;
}

void InitText() {
    REG_DISPCNT = MODE_0 | BG0_ENABLE | BG0_ON;
    REG_BG0CNT = TILE_BASE_0 | SCREEN_BASE_3 | BG_16_COLOR | BG_SIZE_0 | BG_PRIORITY_0;

    // Use DMA to copy font data to video memory
    DMACopy(CHAR_BASE_0, fontTiles, fontTilesLen);
    DMACopy(BG_PALETTE, fontPal, fontPalLen);
    
    // Clear the screen
    ClearText();
}
