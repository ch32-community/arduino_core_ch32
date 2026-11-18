/* Host regression test (run from the repository root):
 * cc -std=c99 -Ilibraries/CH32/examples/CoreMark tests/coremark_timing.c \
 *   libraries/CH32/examples/CoreMark/core_*.c -o /tmp/coremark_timing
 * /tmp/coremark_timing
 */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "coremark.h"

extern volatile ee_s32 seed4_volatile;
extern int coremark_main(void);

static char output[8192];
static size_t output_length;
static uint32_t elapsed_ms;
static uint32_t clock_calls;

uint32_t Arduino_millis(void) {
  return clock_calls++ == 0 ? 0 : elapsed_ms;
}

int ee_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  int length = vsnprintf(output + output_length, sizeof(output) - output_length,
                         format, args);
  va_end(args);
  assert(length >= 0 && (size_t)length < sizeof(output) - output_length);
  output_length += length;
  return length;
}

static void check_output(uint32_t iterations, uint32_t ticks,
                         const char *seconds, const char *score) {
  seed4_volatile = iterations;
  elapsed_ms = ticks;
  clock_calls = 0;
  output_length = 0;
  output[0] = '\0';
  coremark_main();
  assert(strstr(output, seconds));
  if (score) {
    assert(strstr(output, score));
  } else {
    assert(!strstr(output, "Iterations/Sec"));
  }
}

int main(void) {
  const uint64_t values[] = {0, 1, 2, 3, 99, 100, 101, UINT32_MAX,
                            UINT64_C(1) << 32, UINT64_C(1) << 63, UINT64_MAX};
  for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    for (size_t j = 1; j < sizeof(values) / sizeof(values[0]); j++) {
      assert(portable_div_u64(values[i], values[j]) == values[i] / values[j]);
    }
  }
  uint64_t random = 1;
  for (unsigned i = 0; i < 10000; i++) {
    random = random * UINT64_C(6364136223846793005) + 1;
    uint64_t numerator = random;
    random = random * UINT64_C(6364136223846793005) + 1;
    uint64_t divisor = random | 1;
    assert(portable_div_u64(numerator, divisor) == numerator / divisor);
    /* Exercise large quotients as well as equally sized operands. */
    divisor = (uint32_t)random | 1;
    assert(portable_div_u64(numerator, divisor) == numerator / divisor);
  }

  check_output(3000, 14215, "Total time (secs): 14.215\n",
               "Iterations/Sec   : 211.04\n");
  check_output(6000, 14214, "Total time (secs): 14.214\n",
               "Iterations/Sec   : 422.12\n");
  check_output(1, 10000, "Total time (secs): 10.000\n",
               "Iterations/Sec   : 0.10\n");
  check_output(1, 1001, "Total time (secs): 1.001\n",
               "Iterations/Sec   : 1.00\n");
  check_output(1, 6, "Total time (secs): 0.006\n",
               "Iterations/Sec   : 166.67\n");
  check_output(1, UINT32_MAX, "Total time (secs): 4294967.295\n",
               "Iterations/Sec   : 0.00\n");
  check_output(1, 0, "Total time (secs): 0.000\n", NULL);
  printf("CoreMark timing regression tests passed\n\r");
  return 0;
}
