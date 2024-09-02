#ifndef STRING_H
#define STRING_H

#include "stdio.h"

void *memset(void *dst, char c, uint32_t n);

void *memcpy(void *dst, const void *src, uint32_t n);

int memcmp(uint8_t *s1, uint8_t *s2, uint32_t n);

int strcmp(const char *s1, char *s2);

int strcpy(char *dst, const char *src);

void strcat(char *dest, const char *src);

int isspace(char c);

int isalpha(char c);
char upper(char c);
char lower(char c);

void itoa(char *buf, int base, int d);

#endif