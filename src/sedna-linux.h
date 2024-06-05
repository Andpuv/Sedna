# ifndef __SEDNA_LINUX_H
#   define __SEDNA_LINUX_H

#   define __SEDNA_PRIVATE __attribute__((visibility("hidden")))
#   define __SEDNA_PROTECT __attribute__((visibility("default")))
#   define __SEDNA_PUBLIC  __attribute__((visibility("default")))

/* allocators */

__SEDNA_PUBLIC void * sedna_alloc (
  __IN    size_t siz
);

__SEDNA_PUBLIC void * sedna_realloc (
  __INOUT void * ptr
  __IN    size_t siz
);

__SEDNA_PUBLIC void * sedna_dealloc (
  __INOUT void * ptr
);

#   include <stdio.h>
#   include <stdarg.h>
#   include <string.h>

# endif
