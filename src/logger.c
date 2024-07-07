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

static void byte_to_hex_string(uint8_t byte, char **dest) {
  uint8_t upper = (byte & 0xf0) >> 4; 
  uint8_t lower = (byte & 0x0f);
  char *dest_ptr = *dest;
  *dest_ptr       = '0';
  *(dest_ptr + 1) = 'x';
  *(dest_ptr + 2) = hex[upper];
  *(dest_ptr + 3) = hex[lower];
  *(dest_ptr + 4) = ' ';
  *dest += 5;
}

#define LOG_BUFFER_SIZE 255U
static char log_buffer[LOG_BUFFER_SIZE];
void logger_print_bytes_as_hex(const uint8_t *bytes, const uint8_t size) {
  if (size == 0U || ((size * 5) + 2 >= LOG_BUFFER_SIZE)) {
    return;
  }
  char *dest = log_buffer;
  for (size_t i = 0; i < size; i++) {
    byte_to_hex_string(bytes[i], &dest);
  }
  *dest++ = '\n';
  *dest++ = '\0';
  logger(INFO, log_buffer);
}



