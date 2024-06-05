# include <string.h>
# include "sedna.h"

__SEDNA_PRIVATE char * _cfg_read_file (
  __IN    char const * fn
);

__SEDNA_PRIVATE char * _cfg_dup_tok (
  __INOUT struct sedna_cfg_t * cfg,
  __IN    char const *         str,
  __IN    size_t               len
);

__SEDNA_PRIVATE char * _cfg_get_tok (
  __INOUT struct sedna_cfg_t * cfg,
  __INOUT char **              str
);

__SEDNA_PRIVATE int _cfg_add_tok (
  __INOUT struct sedna_cfg_t * cfg,
  __IN    char *               str
);

__SEDNA_PUBLIC int sedna_cfg_ctor (
  __INOUT struct sedna_cfg_t * cfg,
  __IN    char const *         str
)
{
  cfg->argi = 0;
  cfg->argc = 0;
  cfg->argv = NULL;

  if ( !str )
    return SEDNA_FAILURE;

  char * buf = NULL;

  if ( ':' == *str ) {
    buf = _cfg_read_file(str + 1);

    if ( !buf )
      return SEDNA_FAILURE;
  }

  char * cur;

  if ( !buf ) {
    cur = (char *)str;
  } else {
    cur = buf;
  }

  while ( *cur ) {
    char * tok = _cfg_get_tok(cfg, &cur);

    if ( !tok )
      continue;

    if ( _cfg_add_tok(cfg, tok) ) {
      fprintf(stderr, "[ ERROR ] Cannot add token `%s`.\n", tok);
      tok = (char *)sedna_dealloc(tok);
      buf = (char *)sedna_dealloc(buf);
      return SEDNA_FAILURE;
    }
  }

  buf = (char *)sedna_dealloc(buf);
  return SEDNA_SUCCESS;
}

__SEDNA_PUBLIC int sedna_cfg_dtor (
  __INOUT struct sedna_cfg_t * cfg
)
{
  if ( !cfg->argv )
    return SEDNA_FAILURE;

  for ( int argi = 0; argi < cfg->argc; ++argi ) {
    char * args = (char *)sedna_dealloc(
      cfg->argv[ argi ]
    );

    if ( args )
      return SEDNA_FAILURE;

    cfg->argv[ argi ] = args;
  }

  cfg->argv = (char **)sedna_dealloc(
    cfg->argv
  );

  if ( cfg->argv )
    return SEDNA_FAILURE;

  cfg->argi = 0;
  cfg->argc = 0;

  return SEDNA_SUCCESS;
}

__SEDNA_PUBLIC int sedna_cfg_dump (
  __IN    struct sedna_cfg_t * cfg,
  __OUT   FILE *               fp
)
{
  int res = 0;

  res += fprintf(fp, "Configuration at %p:\n", cfg);

  for ( int argi = 0; argi < cfg->argc; ++argi ) {
    char * args = cfg->argv[ argi ];
 
    res += fprintf(fp, "\t%u\t", argi);
    
    if ( !args ) {
      res += fprintf(fp, "NULL\n");
    } else {
      res += fprintf(fp, "`");

      for ( int i = 0; '\0' != args[ i ]; ++i ) {
        char chr [ 4 ];
        int  len = sedna_chr_to_str(chr, args[i]);

        if ( len < 0 )
          continue;

        res += fprintf(fp, "%.*s", len, chr);
      }

      res += fprintf(fp, "`\n");
    }
  }

  return res;
}

__SEDNA_PUBLIC char * sedna_cfg_iter (
  __INOUT struct sedna_cfg_t * cfg,
  __OUT   int *                idx
)
{
  if ( cfg->argi == cfg->argc )
    return NULL;

  char * args = cfg->argv[ cfg->argi ];

  if ( idx ) {
    *idx = cfg->argi;
  }

  ++cfg->argi;

  return args;
}

__SEDNA_PRIVATE char * _cfg_read_file (
  __IN    const char * fn
)
{
  FILE * fp = fopen(fn, "r") ;

  if ( !fp ) {
    fprintf(stderr, "[ ERROR ] Cannot open file `%s`.\n", fn);
    return NULL;
  }

  if ( EOF == fseek(fp, 0, SEEK_END) ) {
    fprintf(stderr, "[ ERROR ] Cannot seek the end of file %s.\n", fn);
    goto _failure;
  }

  long len = ftell(fp);

  if ( len < 0 ) {
    fprintf(stderr, "[ ERROR ] Cannot compute the length of file %s.\n", fn);
    goto _failure;
  }

  if ( EOF == fseek(fp, 0, SEEK_SET) ) {
    fprintf(stderr, "[ ERROR ] Cannot seek the start of file %s.\n", fn);
    goto _failure;
  }

  char * buf = (char *)sedna_alloc(( len + 1 ) * sizeof(char));

  if ( !buf ) {
    fprintf(stderr, "[ ERROR ] Cannot allocate the buffer for file %s.\n", fn);
    goto _failure;
  }

  if ( len != (long)fread(buf, sizeof(char), len, fp) ) {
    fprintf(stderr, "[ ERROR ] Cannot read the entire file %s.\n", fn);
    buf = sedna_dealloc(buf);
    goto _failure;
  }

  buf[ len ] = 0;

  fclose(fp);
  return buf;

_failure:
  fclose(fp);
  return NULL;
}

__SEDNA_PRIVATE char * _cfg_dup_tok (
  __INOUT struct sedna_cfg_t * cfg,
  __IN    char const *         str,
  __IN    size_t               len
)
{
  size_t cnt = 0;
  char * cur = (char *)str;

  for ( cnt = 0; cnt < len; ++cnt ) {
    int chr = sedna_str_to_chr(cur, &cur);

    if ( !chr )
      break;
  }

  char * buf = (char *)sedna_alloc(
    ( cnt + 1 ) * sizeof(char)
  );

  if ( !buf )
    return buf;

  cnt = 0;
  cur = (char *)str;
  
  for ( cnt = 0; cnt < len; ++cnt ) {
    int chr = sedna_str_to_chr(cur, &cur);

    if ( !chr )
      break;

    buf[ cnt ] = chr;
  }

  buf[ cnt ] = 0;

  return buf;
}

__SEDNA_PRIVATE char * _cfg_get_tok (
  __INOUT struct sedna_cfg_t * cfg,
  __INOUT char **              str
)
{
  char * cur = *str;

  while ( '\n' == *cur || '\r' == *cur ) {
    ++cur;
  }

  while ( ' '  == *cur || '\t' == *cur ) {
    ++cur;
  }

  if ( *cur < ' ' || '~' < *cur ) {
    *str = cur;
    return NULL;
  }

  if ( ';' == *cur ) {
    while ( *cur && '\n' != *cur && '\r' != *cur ) {
      ++cur;
    }

    *str = cur;
    return NULL;
  }

  char * tok   = cur;
  char   quote = 0;

  while ( *cur && '\n' != *cur && '\r' != *cur ) {
    if ( !quote ) {
      if ( ' ' == *cur || '\t' == *cur || ';' == *cur )
        break;

      if ( '\'' == *cur || '\"' == *cur ) {
        quote = *cur;
      }
    } else {
      if ( '\\' == *cur && quote == cur[ 1 ] ) {
        ++cur;
      } else if ( quote == *cur ) {
        quote = 0;
      }
    }

    ++cur;
  }

  *str = cur;

  if ( tok == cur )
    return NULL;

  if ( '\'' == *tok || '\"' == *tok ) {
    if ( *tok == cur[ -1 ] )
      return _cfg_dup_tok(cfg, tok + 1, ( (size_t)cur - (size_t)tok ) - 2);
  }

  return _cfg_dup_tok(cfg, tok, (size_t)cur - (size_t)tok);
}

__SEDNA_PRIVATE int _cfg_add_tok (
  __INOUT struct sedna_cfg_t * cfg,
  __IN    char *               str
)
{
  int     argc = cfg->argc + 1;
  char ** argv = (char **)sedna_realloc(
    cfg->argv, argc * sizeof(char *)
  );

  if ( !argv )
    return -1;

  argv[ cfg->argc ] = str;

  cfg->argv = argv;
  cfg->argc = argc;

  return 0;
}
