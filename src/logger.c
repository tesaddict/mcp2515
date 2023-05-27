#include "logger.h"

static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', 
                              '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

typedef struct {
  void (*open)  (void);
  void (*write) (const char*);
  LOG_LEVEL level;
} logger_t;

static logger_t gLogger;

void logger_init(void (*open) (void), void (*write) (const char*), const LOG_LEVEL level)
{
  gLogger.open = open;
  gLogger.open();
  gLogger.write = write;
  gLogger.level = level;
}

void logger(const LOG_LEVEL level, const char *data) {
  if (gLogger.level >= level) {
    gLogger.write(data);
  }
}

void byte_to_hex_string(uint8_t byte, char *dest) {
  uint8_t upper = (byte & 0xf0) >> 4; 
  uint8_t lower = (byte & 0x0f);
  *dest       = hex[upper];
  *(dest + 1) = hex[lower];
}
