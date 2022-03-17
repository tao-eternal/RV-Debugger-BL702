#include <stdarg.h>

// fill it with address of real printf
// take a look at ./readme.md
static void (*p)(const char *__restrict __fmt, ...) = 0x23004296;

int printf(const char *__restrict __fmt, ...) {
  va_list ap;
  va_start(ap, __fmt);
  p(__fmt, ap);
  va_end(ap);
  return 0;
}

int puts(const char *__s) {
  printf(__s);
  return 0;
}
