#include <text.h>
#include <flash.h>
#include <input.h>
#include <stddef.h>
#include "../res/rom.h" // Comment this when testing size

// Uncomment for size testing
// uint8_t rom_gba[1] = {0};
// uint32_t rom_gba_size = 1;

int main() {
    InitText();
    RenderLine("TINYFLASH 1.1 BY HONKEYKONG", currentLine++);
    currentLine++;
    RenderLine("REMOVE THIS CART, INSERT BLANK", currentLine++);
    RenderText("THEN PRESS A", 9, currentLine++);
    
	while (1) {	uint16_t keys = REG_KEYINPUT; if (!(keys & KEY_A)) break; else WaitVBlank(); }

    RenderLine("WRITING TO CART...", currentLine++);
	
    if(QueryCFI()) {
        WriteROMToCartridge(rom_gba, rom_gba_size);
        RenderLine("WRITE COMPLETE!", currentLine++);
    }

    while (1) WaitVBlank();
}