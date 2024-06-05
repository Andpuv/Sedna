# include "sedna.h"
# include <stdlib.h>

__SEDNA_PUBLIC void * sedna_alloc (
  __IN    size_t siz
)
{
  if ( siz ) {
    ptr = malloc(siz);
  }

  return ptr;
}

__SEDNA_PUBLIC void * sedna_realloc (
  __INOUT void * ptr
  __IN    size_t siz
)
{
  if ( siz ) {
    ptr = realloc(ptr, siz);
  } else {
    ptr = sedna_dealloc(ptr);
  }

  return ptr;
}

__SEDNA_PUBLIC void * sedna_dealloc (
  __INOUT void * ptr
)
{
  if ( ptr ) {
    free(ptr);
    ptr = NULL;
  }

  return ptr;
}
