#include <text.h>
#include <flash.h>
#include <input.h>
#include <stddef.h>
#include "../res/rom.h"

int main() {
    InitText();
    RenderLine("TINYFLASH 1.0 BY HONKEYKONG", currentLine++);

    RenderLine("REMOVE THIS CART, INSERT BLANK, THEN PRESS A", currentLine++);
	while (1) {	uint16_t keys = REG_KEYINPUT; if (!(keys & KEY_A)) break; }

    RenderLine("WRITING TO CART...", currentLine++);
	
    if(QueryCFI()) {
        WriteROMToCartridge(rom_gba, rom_gba_size);
        RenderLine("WRITE COMPLETE!", currentLine++);
    }

    while (1);
}