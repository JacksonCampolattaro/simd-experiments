#!/usr/bin/env bash
set -euxo pipefail

# A short script to determine the prevalence of different SIMD instruction sets

# The first argument is the file to analyze
BIN_FILE=$1

# Decompile the bin file
ASM_FILE="$BIN_FILE.asm"
objdump -d "$BIN_FILE" > "$ASM_FILE"

# Determine the count of each type of instruction

TOTAL_COUNT=$(\
    grep ":	" -c "$ASM_FILE" \
)

SSE_COUNT=$(\
    awk '/[ \t](addps|addss|andnps|andps|cmpps|cmpss|comiss|cvtpi2ps|cvtps2pi|cvtsi2ss|cvtss2s|cvttps2pi|cvttss2si|divps|divss|ldmxcsr|maxps|maxss|minps|minss|movaps|movhlps|movhps|movlhps|movlps|movmskps|movntps|movss|movups|mulps|mulss|orps|rcpps|rcpss|rsqrtps|rsqrtss|shufps|sqrtps|sqrtss|stmxcsr|subps|subss|ucomiss|unpckhps|unpcklps|xorps|pavgb|pavgw|pextrw|pinsrw|pmaxsw|pmaxub|pminsw|pminub|pmovmskb|psadbw|pshufw)[ \t]/' \
    "$ASM_FILE" \
    | wc -l \
)

SSE_PACKED_COUNT=$()

SSE2_PACKED_COUNT=$()

SSE3_COUNT=$()

SSSE3_COUNT=$()

SSE4_COUNT=$()

AVX_COUNT=$()

echo $TOTAL_COUNT
echo $SSE_COUNT
