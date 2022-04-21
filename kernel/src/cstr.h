#pragma once
#include <stdint.h>

const char* to_string(uint64_t value);
const char* to_string(int64_t value);
const char* to_hstring(uint64_t value);
const char* to_hstring(uint32_t value);
const char* to_hstring(uint16_t value);
const char* to_hstring(uint8_t value);
const char* to_string(double value, uint8_t decimalPlaces);
const char* to_string(double value);

uint64_t strlen(const char *string);

char* strcpy(char *destination, const char *source);
char* strcat(char *destination, char *source);
unsigned int strlcat(char *__restrict dst, const char *__restrict src, unsigned int dstsize);
void* memcpy(void* dst, const void* src, unsigned int cnt);