#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <stdio.h>

typedef enum {
    INFO    = 0,
    WARNING = 1,
    ERROR   = 2,
    DEBUG   = 3,
    TRACE   = 4
} LOG_LEVEL;

void logger_init(void (*open) (void), void (*write) (const char*), const LOG_LEVEL level);
void logger(const LOG_LEVEL level, const char *data);
void logger_print_bytes_as_hex(const uint8_t *bytes, const uint8_t size);
#endif
