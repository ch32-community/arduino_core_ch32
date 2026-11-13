# TLSF allocator

Unmodified source from [earlephilhower/tlsf at
8653cec85de043454d0e9ddb0957711f6bd14fa1](https://github.com/earlephilhower/tlsf/tree/8653cec85de043454d0e9ddb0957711f6bd14fa1),
the allocator used by Arduino-Pico. The four source files and upstream README
match that commit's Git blob hashes. Copyright and BSD-3-Clause SPDX notices
are retained. See LICENSE for the license text.

The CH32 core includes `tlsf.c` only for CH32V467. Integration, interrupt
locking, maximum-alignment allocation, and newlib routing live in
`cores/arduino/psram.cpp`; they do not modify the vendored implementation.
