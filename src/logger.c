#include "logger.h"


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

static const char HEX_ARRAY[16] = { '0', '1', '2', '3', '4', '5', '6', '7', 
                                    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
#define TEMP_ARRAY_UPPER 2U
#define TEMP_ARRAY_LOWER 3U
static char print_array[] = { '0', 'x', '\0', '\0', ' ' };
static const size_t PRINT_ARRAY_SIZE = sizeof(print_array) / sizeof(char);
static void byte_to_hex_string(uint8_t byte, char **dest) {
  print_array[TEMP_ARRAY_UPPER] = HEX_ARRAY[(byte & 0xf0) >> 4];
  print_array[TEMP_ARRAY_LOWER] = HEX_ARRAY[(byte & 0x0f)];
  char *dest_ptr = *dest;
  for (size_t idx = 0; idx < PRINT_ARRAY_SIZE; idx++) {
    dest_ptr[idx] = print_array[idx];
  }
  *dest += PRINT_ARRAY_SIZE;
}

#define LOG_BUFFER_SIZE 255U
static char log_buffer[LOG_BUFFER_SIZE];
void logger_print_bytes_as_hex(const uint8_t *bytes, const uint8_t size) {
  if (size == 0U || (size * PRINT_ARRAY_SIZE + 2 >= LOG_BUFFER_SIZE)) {
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



