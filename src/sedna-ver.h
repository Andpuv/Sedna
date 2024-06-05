# ifndef __SEDNA_VER_H
#   define __SEDNA_VER_H

#   define __SEDNA_VERSION_MAJOR 0
#   define __SEDNA_VERSION_MINOR 0
#   define __SEDNA_VERSION_PATCH 0
#   define __SEDNA_VERSION              \
    (                                   \
      ( __SEDNA_VERSION_MAJOR << 24 ) | \
      ( __SEDNA_VERSION_MAJOR << 16 ) | \
      ( __SEDNA_VERSION_MAJOR <<  0 )   \
    )

# endif
