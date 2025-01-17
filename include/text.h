#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>
#include <stdbool.h>
#include "../res/font.h"

#define MODE_0 0
#define BG0_ON (1 << 8)
#define BG_PRIORITY_0 0
#define BG_SIZE_0 (0 << 14)
#define BG0_ENABLE (1 << 8)
#define TILE_BASE_0 (0 << 2)
#define BG_16_COLOR (0 << 7)
#define SCREEN_BASE_3 (3 << 8)
#define BG_PALETTE ((uint16_t *)0x05000000)
#define MAP_BASE_3 ((void*)0x6000000 + (3 << 11))
#define CHAR_BASE_0 ((void *)(0x6000000 + (0 << 14)))
#define REG_BG0CNT	*((volatile uint16_t *)0x04000008)
#define REG_DISPCNT *((volatile uint16_t *)0x04000000)

#define REG_DMA3SAD   *(volatile uint32_t*)0x40000D4 // DMA3 Source Address
#define REG_DMA3DAD   *(volatile uint32_t*)0x40000D8 // DMA3 Destination Address
#define REG_DMA3CNT_L *(volatile uint16_t*)0x40000DC // DMA3 Control Low (Length)
#define REG_DMA3CNT_H *(volatile uint16_t*)0x40000DE // DMA3 Control High (Control Flags)

void InitText();
void ClearText();
void CheckLines();
const char* IntToASCII(int value);
const char* HexString(uint16_t value);
uint32_t stringLength(const char *str);
void RenderLine(const char *text, uint8_t line);
void RenderText(const char* text, uint8_t x, uint8_t y);
void DMACopy(void* dest, const void* src, uint32_t size);
void RenderHex(unsigned int value, uint8_t x, uint8_t y);

#define RENDER_LINE_WITH_VALUE(msg, value) \
    RenderLine(msg, currentLine); \
    RenderText(IntToASCII(value), stringLength(msg), currentLine++);

extern uint8_t currentLine;

#endif // TEXT_H