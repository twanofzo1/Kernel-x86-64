#pragma once
#include <types.h>

#define BIT_0  (1<<0 )
#define BIT_1  (1<<1 )
#define BIT_2  (1<<2 )
#define BIT_3  (1<<3 )
#define BIT_4  (1<<4 )
#define BIT_5  (1<<5 )
#define BIT_6  (1<<6 )
#define BIT_7  (1<<7 )
#define BIT_8  (1<<8 )
#define BIT_9  (1<<9 )
#define BIT_10 (1<<10)
#define BIT_11 (1<<11)
#define BIT_12 (1<<12)
#define BIT_13 (1<<13)
#define BIT_14 (1<<14)
#define BIT_15 (1<<15)
#define BIT_16 (1<<16)

#define GET_BIT_8(byte, n)              ((byte) & (1 << (n)))
#define SET_BIT_8(byte, n, enabled)     ((enabled) ? ((byte) | (1 << (n))) : ((byte) & ~(1 << (n))))

#define GET_BIT_16(word, n)             ((word) & (1 << (n)))
#define SET_BIT_16(word, n, enabled)    ((enabled) ? ((word) | (1 << (n))) : ((word) & ~(1 << (n))))

#define GET_LOWER_8(total)              ((u8)((total) & 0xFF))
#define GET_HIGHER_8(total)             ((u8)((total) >> 8))

#define GET_LOWER_16(total)             ((u16)((total) & 0xFFFF))
#define GET_HIGHER_16(total)            ((u16)((total) >> 16))

#define MAKE_16(lower, higher)          ((u16)((lower) | ((higher) << 8)))
#define MAKE_32(lower, higher)          ((u32)((lower) | ((higher) << 16)))