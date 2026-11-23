#define ARDUINO_MAIN

#include "Arduino.h"
#include "debug.h"

// The core archive is not linked with --whole-archive (see platform.txt), and
// libc's free()/realloc() -- which reach psram.cpp's --wrap targets -- can be
// pulled in by libraries searched after it. Referencing the wrappers from
// this always-linked file extracts psram.cpp from the archive while it is
// still being searched. The reference is in its own section, which
// --gc-sections discards, so a sketch that never frees memory pays nothing.
extern "C" void __wrap__free_r(struct _reent *, void *);
extern "C" void *__wrap__realloc_r(struct _reent *, void *, size_t);
__attribute__((used, section(".rodata.psram_wrap_anchor")))
static const void *const psram_wrap_anchor[] = {
    (const void *)__wrap__free_r,
    (const void *)__wrap__realloc_r,
};


/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
    pre_init( );
#if defined(CH32V467)
    psramInit();
#endif
#if defined(USE_TINYUSB)
    if (TinyUSB_Device_Init) {
        TinyUSB_Device_Init(0);
    }
#endif
    setup( );
  
    do {
        loop( );
#if defined(USE_TINYUSB)
        if (TinyUSB_Device_Task) {
            TinyUSB_Device_Task();
        }
        if (TinyUSB_Device_FlushCDC) {
            TinyUSB_Device_FlushCDC();
        }
#endif
    } while (1);

    return 0;
}
