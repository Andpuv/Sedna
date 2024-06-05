# include "sedna.h"
# include <string.h>
# include <errno.h>

__SEDNA_PUBLIC int sedna_str_to_uint (
  __IN    char const * str,
  __OUT   char **      endptr,
  __IN    int          rad,
  __IN    u_long_t *   num
)
{
  if ( !str )
    return -1;

  unsigned long long res = strtoull(str, endptr, rad);

  if ( !res || ULONG_MAX == res ) {
    if ( ERANGE == errno )
      return -2;
  }

  if ( num ) {
    *num = (u_long_t)res;
  }

  return 0;
}

__SEDNA_PUBLIC int sedna_str_to_sint (
  __IN    char const * str,
  __OUT   char **      endptr,
  __IN    int          rad,
  __IN    s_long_t *   num
)
{
  if ( !str )
    return -1;

  signed long long res = strtoll(str, endptr, rad);

  if ( !res || LONG_MIN == res || LONG_MAX == res ) {
    if ( ERANGE == errno )
      return -2;
  }

  if ( num ) {
    *num = (s_long_t)res;
  }

  return 0;
}

__SEDNA_PUBLIC int sedna_str_to_chr (
  __IN    char const * str,
  __OUT   char **      endptr
)
{
  char * cur = (char *)str;
  int    chr = *cur++;

  if ( '\\' == chr ) {
    chr = *cur++;

    switch ( chr ) {
    case 'a'  : { chr = '\a';   } break;
    case 'c'  : { chr = '\b';   } break;
    case 't'  : { chr = '\t';   } break;
    case 'n'  : { chr = '\n';   } break;
    case 'v'  : { chr = '\v';   } break;
    case 'f'  : { chr = '\f';   } break;
    case 'r'  : { chr = '\r';   } break;
    case 'e'  : { chr = '\x1B'; } break;
    case 's'  : { chr = ' ';    } break;
    case '!'  : { chr = '!';    } break;
    case '?'  : { chr = '?';    } break;
    case '\'' : { chr = '\'';   } break;
    case '\"' : { chr = '\"';   } break;
    case 'g'  : { chr = '`';    } break;
    case 'd'  : { chr = '\x7F'; } break;

    case '0'  :
    case 'b'  : case 'B' :
    case 'x'  : case 'X' :
    default   : {
      int rad =
        ( '0' == chr               ) *  8 +
        ( 'b' == chr || 'B' == chr ) *  2 +
        ( 'x' == chr || 'X' == chr ) * 16;

      if ( !rad ) {
        rad = 10;
      }

      u_word_t num;

      if ( sedna_str_to_uint(cur, &cur, rad, &num) )
        return -1;

      chr = (char)( num & 0xFF );
    } break;
    }
  }

  if ( endptr ) {
    *endptr = cur;
  }

  return chr;
}

__SEDNA_PUBLIC int sedna_chr_to_str (
  __OUT   char * str,
  __IN    int    chr
)
{
  static const char hex [] = "0123456789ABCDEF";

  int n = 0;

  switch ( chr ) {
  case '\a'   : { str[ n++ ] = '\\'; str[ n++ ] = 'a';  } break;
  case '\b'   : { str[ n++ ] = '\\'; str[ n++ ] = 'c';  } break;
  case '\t'   : { str[ n++ ] = '\\'; str[ n++ ] = 't';  } break;
  case '\n'   : { str[ n++ ] = '\\'; str[ n++ ] = 'n';  } break;
  case '\v'   : { str[ n++ ] = '\\'; str[ n++ ] = 'v';  } break;
  case '\f'   : { str[ n++ ] = '\\'; str[ n++ ] = 'f';  } break;
  case '\r'   : { str[ n++ ] = '\\'; str[ n++ ] = 'r';  } break;
  case '\x1B' : { str[ n++ ] = '\\'; str[ n++ ] = 'e';  } break;
  case ' '    : { str[ n++ ] = '\\'; str[ n++ ] = 's';  } break;
  case '!'    : { str[ n++ ] = '\\'; str[ n++ ] = '!';  } break;
  case '?'    : { str[ n++ ] = '\\'; str[ n++ ] = '?';  } break;
  case '\''   : { str[ n++ ] = '\\'; str[ n++ ] = '\''; } break;
  case '\"'   : { str[ n++ ] = '\\'; str[ n++ ] = '\"'; } break;
  case '`'    : { str[ n++ ] = '\\'; str[ n++ ] = 'g';  } break;
  case '\x7F' : { str[ n++ ] = '\\'; str[ n++ ] = 'd';  } break;
  default     : {
    if ( ' ' < chr && chr <= '~' ) {
      str[ n++ ] = chr;
    } else {
      str[ n++ ] = '\\';
      str[ n++ ] = 'x';
      str[ n++ ] = hex[ ( chr >> 4 ) & 0xF ];
      str[ n++ ] = hex[ ( chr >> 0 ) & 0xF ];
    }
  } break;
  }

  return n;
}
