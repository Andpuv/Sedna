# ifndef __SEDNA_H
#   define __SEDNA_H

#   include <stdint.h>
#   include <stddef.h>
#   include <stdlib.h>
#   include <stdio.h>

#   define __IN
#   define __OUT
#   define __INOUT

#   ifndef false
#     define false ( 0 != 0 )
#   endif

#   ifndef true
#     define true  ( 0 == 0 )
#   endif

typedef uint8_t  u_byte_t;
typedef uint16_t u_half_t;
typedef uint32_t u_word_t;
typedef uint64_t u_long_t;

#   define U_BYTE(n)  UINT8_C(n)
#   define U_HALF(n)  UINT16_C(n)
#   define U_WORD(n)  UINT32_C(n)
#   define U_LONG(n)  UINT64_C(n)

#   define U_BYTE_MAX UINT8_MAX
#   define U_HALF_MAX UINT16_MAX
#   define U_WORD_MAX UINT32_MAX
#   define U_LONG_MAX UINT64_MAX

typedef  int8_t  s_byte_t;
typedef  int16_t s_half_t;
typedef  int32_t s_word_t;
typedef  int64_t s_long_t;

#   define S_BYTE(n)  INT8_C(n)
#   define S_HALF(n)  INT16_C(n)
#   define S_WORD(n)  INT32_C(n)
#   define S_LONG(n)  INT64_C(n)

#   define S_BYTE_MIN INT8_MIN
#   define S_HALF_MIN INT16_MIN
#   define S_WORD_MIN INT32_MIN
#   define S_LONG_MIN INT64_MIN

#   define S_BYTE_MAX INT8_MAX
#   define S_HALF_MAX INT16_MAX
#   define S_WORD_MAX INT32_MAX
#   define S_LONG_MAX INT64_MAX

#   define Ki(n) ( ((u_long_t)n) << 10 )
#   define Mi(n) ( ((u_long_t)n) << 20 )
#   define Gi(n) ( ((u_long_t)n) << 30 )
#   define Ti(n) ( ((u_long_t)n) << 40 )
#   define Pi(n) ( ((u_long_t)n) << 50 )
#   define Ei(n) ( ((u_long_t)n) << 60 )

struct sedna_cfg_t;
struct sedna_obj_t;

#   include "sedna-ver.h"
#   include "sedna-def.h"
#   include "sedna-cfg.h"
#   include "sedna-obj.h"
#   include "sedna-ram.h"

#   define __SEDNA__ __SEDNA_VERSION

# endif
