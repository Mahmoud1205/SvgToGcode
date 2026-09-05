#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <assert.h>

// TODO: real SBREAK
#define SBREAK() __debugbreak()

#define ALIGN(inAlign)		__attribute__((aligned(inAlign)))

#define likely(inCond)		__builtin_expect(!!(inCond), 1)
#define unlikely(inCond)	__builtin_expect(!!(inCond), 0)

#define CONCAT_DETAIL(x, y) x##y
#define CONCAT(x, y) CONCAT_DETAIL(x, y)

// shorthand macros for printing to stderr
#define eputs(inTxt) fputs(inTxt "\n", stderr)
#define eprintf(inTxt, ...) fprintf(stderr, inTxt, ##__VA_ARGS__)

#define CLAMP(inVal, inMin, inMax) \
	((inVal) < (inMin) ? (inMin) : ((inVal) > (inMax) ? (inMax) : (inVal)))

#define MAX(inA, inB) ((inA > inB) ? inA : inB)
#define MIN(inA, inB) ((inA < inB) ? inA : inB)

// bitset macros
#define BSET(inBitset, inIdx)		do {	inBitset |=		(1 << inIdx);	} while (0)
#define BTEST(inBitset, inIdx)		(		inBitset &		(1 << inIdx)	)
#define BCLEAR(inBitset, inIdx)		do {	inBitset &=		~(1 << inIdx);	} while (0)
#define BTOGGLE(inBitset, inIdx)	do {	inBitset ^=		(1 << inIdx);	} while (0)

// NOTE(Mahmoud): removed in this version because no reinterpret cast in C
// these work with any type variable, even floats
//#define BPRINT8(inVar)  do { for (I32 i = 7;  i >= 0; i--) putchar(BTEST(reinterpret_cast<U8>(inVar), i)  ? '1' : '0'); } while (0);
//#define BPRINT16(inVar) do { for (I32 i = 15; i >= 0; i--) putchar(BTEST(reinterpret_cast<U16>(inVar), i) ? '1' : '0'); } while (0);
//#define BPRINT32(inVar) do { for (I32 i = 31; i >= 0; i--) putchar(BTEST(reinterpret_cast<U32>(inVar), i) ? '1' : '0'); } while (0);
//#define BPRINT64(inVar) do { for (I32 i = 63; i >= 0; i--) putchar(BTEST(reinterpret_cast<U64>(inVar), i) ? '1' : '0'); } while (0);

/// @brief this has no functionality, it is only used for code clarity
#define bitset

#ifndef M_PI
#	define M_PI 3.141592654
#endif

#define OFFSETOF __builtin_offsetof

#define CONTAINER(inPtr, inType, inMember) \
	((inType*)((U8*)(inPtr) - offsetof(inType, inMember)))

#define UNUSED(inVar) (void)inVar

#define ARRAYSIZE(inArr) (sizeof(inArr) / sizeof((inArr)[0]))

/// @brief Zero a pointer with a size.
#define MEMZERO(inPtr, inSize) memset(inPtr, 0, inSize)

/// @brief Zero a non-pointer variable.
#define SMEMZERO(inVar) memset(&inVar, 0, sizeof(inVar))

#define PAUSE() _mm_pause()

typedef uintptr_t		uptr;
typedef intptr_t		iptr;
typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef size_t			usize;
typedef iptr			isize;

static_assert(sizeof(usize) == sizeof(isize));

typedef int8_t I8;
static_assert(sizeof(I8) == 1, "Expected I8 to be 1 byte.");

typedef int_fast8_t I8F;
static_assert(sizeof(I8F) >= 1, "Expected I8F to be at least 1 byte.");

typedef int16_t I16;
static_assert(sizeof(I16) == 2, "Expected I16 to be 2 bytes.");

typedef int_fast16_t I16F;
static_assert(sizeof(I16F) >= 2, "Expected I16F to be at least 2 bytes.");

typedef int I32;
static_assert(sizeof(I32) == 4, "Expected I32 to be 4 bytes.");

typedef int64_t I64;
static_assert(sizeof(I64) == 8, "Expected I64 to be 8 bytes.");

typedef uint8_t U8;
static_assert(sizeof(U8) == 1, "Expected U8 to be 1 byte.");

typedef uint_fast8_t U8F;
static_assert(sizeof(U8F) >= 1, "Expected U8F to be at least 1 byte.");

typedef uint16_t U16;
static_assert(sizeof(U16) == 2, "Expected U16 to be 2 bytes.");

typedef uint_fast16_t U16F;
static_assert(sizeof(U16F) >= 2, "Expected U16F to be at least 2 bytes.");

typedef uint U32;
static_assert(sizeof(U32) == 4, "Expected U32 to be 4 bytes.");

typedef uint64_t U64;
static_assert(sizeof(U64) == 8, "Expected U64 to be 8 bytes.");

typedef float F32;
static_assert(sizeof(F32) == 4, "Expected F32 to be 4 bytes.");

typedef double F64;
static_assert(sizeof(F64) == 8, "Expected F64 to be 8 bytes.");

// for code clarity to differentiate between bitsets and numbers
typedef U8  B8;
typedef U16 B16;
typedef U32 B32;
typedef U64 B64;

static_assert(sizeof(bool) == 1, "Expected bool to be 1 byte.");

#define SIZE_KB(inSize) ((usize)(inSize * 1024))
#define SIZE_MB(inSize) ((usize)(inSize * 1024 * 1024))
#define SIZE_GB(inSize) ((usize)(inSize * 1024 * 1024 * 1024))

#define FSIZE_KB(inSize) ((F32)inSize * 1024.0f)
#define FSIZE_MB(inSize) ((F32)inSize * 1024.0f * 1024.0f)
#define FSIZE_GB(inSize) ((F32)inSize * 1024.0f * 1024.0f * 1024.0f)

#define BYTES_KB(inSize) ((F32)inSize / 1024.0f)
#define BYTES_MB(inSize) ((F32)inSize / 1024.0f / 1024.0f)
#define BYTES_GB(inSize) ((F32)inSize / 1024.0f / 1024.0f / 1024.0f)

// max unsigned
#define MAXU8	UINT8_MAX
#define MAXU16	UINT16_MAX
#define MAXU32	UINT32_MAX
#define MAXU64	UINT64_MAX

// max signed
#define MAXI8	INT8_MAX
#define MAXI16	INT16_MAX
#define MAXI32	INT32_MAX
#define MAXI64	INT64_MAX

// min signed
#define MINI8	INT8_MIN
#define MINI16	INT16_MIN
#define MINI32	INT32_MIN
#define MINI64	INT64_MIN
