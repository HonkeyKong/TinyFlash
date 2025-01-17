#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

void FlashROM();
bool QueryCFI();
uint16_t DetectChipType();
uint8_t readByte(int addr);
void EraseSector(uint32_t address);
void WriteData(uint32_t address, const uint8_t* data, uint32_t size);
bool VerifyData(uint32_t address, const uint8_t* data, uint32_t size);
void WriteROMToCartridge(const uint8_t* romData, uint32_t romSize);

#endif // FLASH_H