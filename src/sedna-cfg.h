# ifndef __SEDNA_CFG_H
#   define __SEDNA_CFG_H

#   include <stdio.h>

struct sedna_cfg_t {
  int     argi;
  int     argc;
  char ** argv;
};

__SEDNA_PUBLIC int sedna_cfg_ctor (
  __INOUT struct sedna_cfg_t * cfg,
  __IN    char const *         str
);

__SEDNA_PUBLIC int sedna_cfg_dtor (
  __INOUT struct sedna_cfg_t * cfg
);

__SEDNA_PUBLIC char * sedna_cfg_dump (
  __IN    struct sedna_cfg_t * cfg,
  __OUT   FILE *               fp
);

__SEDNA_PUBLIC char * sedna_cfg_iter (
  __INOUT struct sedna_cfg_t * cfg,
  __OUT   int *                idx
);

# endif
