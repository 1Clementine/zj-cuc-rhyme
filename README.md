# Rhyme-SM3 Signature Submission Package

SM3-backend implementation of the Rhyme signature algorithm —
reference and optimized — for the Next-generation Commercial
Cryptographic Algorithms Program (NGCC).

## Contents

```
Rhyme-SM3/
├── Implementations/
│   ├── Reference_Implementation/
│   │   ├── Rhyme-SM3-128/   Rhyme-SM3-256/
│   │   ├── Rhyme-SM3-384/   Rhyme-SM3-512/
│   ├── Optimized_Implementation/
│   │   ├── Rhyme-SM3-128/   Rhyme-SM3-256/
│   │   ├── Rhyme-SM3-384/   Rhyme-SM3-512/
│   │   └── README
│   ├── Additional_Implementation/
│   └── README
├── Test_Vectors/
│   ├── KAT_SIG_Rhyme-SM3-128.txt
│   ├── KAT_SIG_Rhyme-SM3-256.txt
│   ├── KAT_SIG_Rhyme-SM3-384.txt
│   └── KAT_SIG_Rhyme-SM3-512.txt
└── README.md
```

## Algorithm

Rhyme is a Fiat-Shamir lattice-based digital signature algorithm with a
compressed unimodular ("cut-F") construction.

**Backend:** SM3 / SM3-KDF (pseudoXOF) via `rhyme_xof.c/h` → `sm3_xof.c` →
API_PKC `auxfunc.c`.  No fips202 dependency.

SIG only — no KEM/KEX.  API_PKC `no modify/` files byte-identical.

## Parameter Sets

| Instance       | Security | N    | Q     | K | L |
|----------------|----------|------|-------|---|---|
| Rhyme-SM3-128  | 128-bit  | 256  | 3329  | 2 | 3 |
| Rhyme-SM3-256  | 256-bit  | 512  | 9473  | 2 | 3 |
| Rhyme-SM3-384  | 384-bit  | 512  | 11777 | 3 | 4 |
| Rhyme-SM3-512  | 512-bit  | 1024 | 18433 | 2 | 3 |

## Quick Start

```bash
# Reference (portable C99, SM3 backend)
cd Implementations/Reference_Implementation/Rhyme-SM3-128
make clean && make && ./KAT_SIG

# Optimized default (incremental SM3-XOF, KAT byte-identical to Reference)
cd Implementations/Optimized_Implementation/Rhyme-SM3-128
make clean && make && ./KAT_SIG

# Optimized fast (DP1 keygen, ~8-14x faster, KAT differs)
make clean && make fast && ./KAT_SIG_FAST
```

See `Implementations/Optimized_Implementation/README` for details.
