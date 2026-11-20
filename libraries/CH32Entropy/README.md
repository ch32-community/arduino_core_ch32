# Experimental CH32 entropy

Local prototype for CH32V407/467. **Not validated for production security.**
The observed hardware RNG output is correlated and its minimum entropy has not
been established. SHA-256 and a software RNG cannot manufacture missing entropy.

## TLS security limitation

The CH32 hardware RNG has shown poor output quality, including substantial
repetition and correlation. This implementation collects and conditions that
output, but has no established lower bound on unpredictable seed bits. Reading
more words does not by itself establish that the seed is secure.

TLS connections can complete and validate server certificates while their
locally generated secrets remain insufficiently unpredictable. If an attacker
can predict the generator state, TLS confidentiality can be compromised even
when certificate checks pass. This is an unverified security property, not a
known percentage reduction in randomness or security. Keep this implementation
experimental; do not rely on it alone to protect sensitive data or credentials.

The experimental opt-in and initialization checks remain required. A successful
`ch32EntropyReady()` result reports operational readiness, not secure seeding.

## Usage

This optional core-bundled library reuses the SHA-256 and HMAC_DRBG implementation
in an unmodified ESP_SSLClient installation. It does not alter Arduino
`random()` or `randomSeed()` and does not add a dependency to ordinary sketches.

For an experimental build, add `-DCH32_ALLOW_EXPERIMENTAL_ENTROPY=1` to both C and
C++ compiler flags, and `-DBR_USE_GETENTROPY=1` to the C compiler flags. Clean
rebuild the library so its existing BearSSL system seeder uses `getentropy()`.
Include `CH32Entropy.h` and call `ch32EntropyBegin()` from `setup()`, with
interrupts enabled. It discards 4096 startup words, waits 100 ms, then hashes
1024 readings spaced at least 50 us apart into the software RNG seed.

`ch32EntropyReady()` means initialization succeeded, not that the source has a
proven entropy budget. Check readiness before **every** TLS connection and do
not connect after initialization failure. ESP_SSLClient also injects Arduino
`random()` bytes, so it will not reliably refuse TLS merely because the system
seeder failed. This explicit application check is required. Once initialized,
the system hook uses software state, with no hardware reads during handshakes.
Do not end or reinitialize the generator while a connection is active.

`getentropy()` accepts at most 256 bytes and leaves the buffer unchanged on
failure. `ch32EntropyEnd()` erases the generator state. `ch32EntropyStatus()`
reports initialization failures; `ch32EntropyRequests()` counts successful output
requests without exposing seeds. These APIs are for single-threaded main-loop
use, not interrupts. Reseeding explicitly replaces the old state.

Eight consecutive identical words, fewer than 128 distinct words per pool,
hardware error flags, and bounded-wait timeouts reject initialization. These
are gross-fault checks, **not** calibrated SP 800-90B health tests. No samples,
digests, or generator state are logged or stored in flash.
