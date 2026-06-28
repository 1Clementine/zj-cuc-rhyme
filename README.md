# Rhyme-SHAKE Signature

SHAKE-backend implementation of the Rhyme signature algorithm —
reference and optimized — for the Next-generation Commercial
Cryptographic Algorithms Program (NGCC).

## Contents

```
Rhyme-SHAKE/
├── Implementations/
│   ├── Reference_Implementation/
│   │   ├── Rhyme-SHAKE-128/   Rhyme-SHAKE-256/
│   │   ├── Rhyme-SHAKE-384/   Rhyme-SHAKE-512/
│   ├── Optimized_Implementation/
│   │   ├── Rhyme-SHAKE-128/   Rhyme-SHAKE-256/
│   │   ├── Rhyme-SHAKE-384/   Rhyme-SHAKE-512/
│   │   └── README
│   ├── Additional_Implementation/
│   └── README
├── Test_Vectors/
│   ├── KAT_SIG_Rhyme-SHAKE-128.txt
│   ├── KAT_SIG_Rhyme-SHAKE-256.txt
│   ├── KAT_SIG_Rhyme-SHAKE-384.txt
│   └── KAT_SIG_Rhyme-SHAKE-512.txt
└── README.md
```

## Algorithm

Rhyme is a Fiat-Shamir lattice-based digital signature algorithm with a
compressed unimodular ("cut-F") construction.

**Backend:** SHAKE-128/256 (FIPS 202) via `rhyme_xof.c/h` → `fips202.c`.

SIG only — no KEM/KEX.  API_PKC `no modify/` files byte-identical.

## Parameter Sets

| Instance         | Security | N    | Q     | K | L |
|------------------|----------|------|-------|---|---|
| Rhyme-SHAKE-128  | 128-bit  | 256  | 3329  | 2 | 3 |
| Rhyme-SHAKE-256  | 256-bit  | 512  | 9473  | 2 | 3 |
| Rhyme-SHAKE-384  | 384-bit  | 512  | 11777 | 3 | 4 |
| Rhyme-SHAKE-512  | 512-bit  | 1024 | 18433 | 2 | 3 |

## Quick Start

```bash
# Reference (portable C99)
cd Implementations/Reference_Implementation/Rhyme-SHAKE-128
make clean && make && ./KAT_SIG

# Optimized default (KAT byte-identical to Reference)
cd Implementations/Optimized_Implementation/Rhyme-SHAKE-128
make clean && make && ./KAT_SIG

# Optimized fast (DP1 keygen, ~8-14x faster, KAT differs)
make clean && make fast && ./KAT_SIG_FAST
```

See `Implementations/Optimized_Implementation/README` for DP1 details.
