#include "logger.h"

logger_t* logger_alloc(
        void (*open) (void), 
        void (*write) (const char*), 
        const LOG_LEVEL level)
{
    logger_t *logger_ptr = malloc(sizeof(logger_t));
    logger_ptr->open = open;
    logger_ptr->open();
    logger_ptr->write = write;
    logger_ptr->level = level;
    return logger_ptr;
}

void logger_free(logger_t *logger_ptr) {
    if (logger_ptr == NULL) return;
    free(logger_ptr);
    logger_ptr = NULL;
}

void logger_log(const logger_t *logger, const LOG_LEVEL level, const char *data) {
    if (logger->level >= level) {
        logger->write(data);
    }
}

void byte_to_hex_string(uint8_t byte, char *dest) {
    static const char hex[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', 
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    uint8_t upper = (byte & 0xf0) >> 4; 
    uint8_t lower = (byte & 0x0f);
    *dest       = hex[upper];
    *(dest + 1) = hex[lower];
}
