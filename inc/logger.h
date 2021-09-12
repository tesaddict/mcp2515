#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    INFO    = 0,
    WARNING = 1,
    ERROR   = 2,
    DEBUG   = 3,
    TRACE   = 4
} LOG_LEVEL;

typedef struct {
    void (*open)  (void);
    void (*write) (const char*);
    LOG_LEVEL level;
} logger_t;

logger_t* logger_alloc(
              void (*open) (void), 
              void (*write) (const char*), 
              const LOG_LEVEL level);
void      logger_log(
              const logger_t *logger,
              const LOG_LEVEL level,
              const char *data);
void     logger_free(logger_t *logger_ptr);
void     byte_to_hex_string(uint8_t byte, char *dest);
#endif
