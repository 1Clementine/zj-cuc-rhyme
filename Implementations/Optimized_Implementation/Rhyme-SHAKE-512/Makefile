# Rhyme-SHAKE Optimized Implementation Makefile
# Instance: Rhyme-SHAKE-512
#
# Targets:
#   make        -> KAT_SIG (default, KAT byte-identical to Reference)
#   make fast   -> KAT_SIG_FAST (USE_DP1_CHECK, faster keygen, different KAT)

INST := Rhyme-SHAKE-512
MODE := 512

CC      ?= gcc
CFLAGS  ?= -O3 -march=native -std=c99 -Wall -Wextra -I. -Iinclude -Isrc/keygen \
           -DRHYME_NO_AES \
           -DRHYME_MODE=$(MODE) \
           -DOUTPUT_BLANK_TEST_VECTORS=0 \
           -DALGORITHM_INSTANCE='"$(INST)"'
LDLIBS   = -lm

# Core Rhyme sources (SHAKE backend)
CORE = src/poly.c \
       src/ntt.c \
       src/ntt_tables.c \
       src/sampler.c \
       src/encoding.c \
       src/packing.c \
       src/sign.c \
       src/zpntt.c \
       src/symmetric-shake.c \
       src/fips202.c \
       rhyme_xof.c \
       src/keygen/kg_main.c \
       src/keygen/kg_solver.c \
       src/keygen/kg_zint.c \
       src/keygen/kg_ntt.c \
       src/keygen/kg_primes.c

KAT_DEPS = randombytes.c auxfunc.c drng.c SIG_AlgorithmInstance.c KAT_SIG.c

SRCS = $(CORE) $(KAT_DEPS)

all: KAT_SIG

KAT_SIG: $(SRCS)
	$(CC) $(CFLAGS) -o KAT_SIG $(SRCS) $(LDLIBS)

fast: KAT_SIG_FAST

KAT_SIG_FAST: $(SRCS)
	$(CC) $(CFLAGS) -DUSE_DP1_CHECK -o KAT_SIG_FAST $(SRCS) $(LDLIBS)

clean:
	rm -f KAT_SIG KAT_SIG_FAST *.o src/*.o src/keygen/*.o
