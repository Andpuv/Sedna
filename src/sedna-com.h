# ifndef __SEDNA_COM_H
#   define __SEDNA_COM_H

#   define __SEDNA_DEBUG

#   ifdef __SEDNA_DEBUG
#     define __SEDNA_REQUIRES_TRACKING
#     define __SEDNA_REQUIRES_VERBOSITY
#   endif

#   define __IN
#   define __OUT
#   define __INOUT

#   if defined(__linux__) || defined(__linux) || defined(linux)
#     define __SEDNA_OS_LINUX
#   else
#     error: "unrecognized operating system (Linux-based only)".
#   endif

#   if defined(__SEDNA_OS_LINUX)
#     include "sedna-linux.h"
#   endif

#   define SEDNA_FAILURE -1
#   define SEDNA_SUCCESS  0
#   define SEDNA_WAITING +1

__SEDNA_PUBLIC int sedna_str_to_uint (
  __IN    char const * str,
  __OUT   char **      endptr,
  __IN    int          rad,
  __IN    u_long_t *   num
);

__SEDNA_PUBLIC int sedna_str_to_sint (
  __IN    char const * str,
  __OUT   char **      endptr,
  __IN    int          rad,
  __IN    s_long_t *   num
);

__SEDNA_PUBLIC int sedna_str_to_chr (
  __IN    char const * str,
  __OUT   char **      endptr
);

__SEDNA_PUBLIC int sedna_chr_to_str (
  __IN    char * str,
  __IN    int    chr
);

# endif
