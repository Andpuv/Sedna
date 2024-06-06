# include <inttypes.h>
# include <string.h>
# include <errno.h>
# include "sedna.h"

# define _ram_dtor()                            \
    ram = (struct sedna_obj_t *)sedna_obj_dtor( \
      (struct sedna_obj_t *)ram                 \
    )

__SEDNA_PUBLIC struct sedna_ram_t * sedna_ram_ctor (
  __INOUT struct sedna_ram_t * ram,
  __IN    struct sedna_cfg_t * cfg
)
{
  ram = (struct sedna_obj_t *)sedna_obj_ctor(
    (struct sedna_obj_t *)ram, sizeof(struct sedna_ram_t)
  );

  if ( !ram )
    return ram;

  ram->len = SEDNA_RAM_DEF_LEN;
  ram->buf = (u_byte_t *)NULL;

  char * args;

  while ( args = sedna_cfg_iter(cfg, NULL) ) {
    if (
      0 == strcmp(args, "-l")    ||
      0 == strcmp(args, "--len")
    ) {
      char * len_str = sedna_cfg_iter(cfg, NULL);

      if ( !len_str ) {
        fprintf(
          stderr,
          "[ ERROR ] Cannot configure the RAM length:\n"
          "          missing configuration parameter `%s LENGTH`.\n"
        );

        _ram_dtor();
        return NULL;
      }

      u_long_t len_num;

      if ( sedna_str_to_size(len_str, NULL, 0, &len_num) ) {
        fprintf(
          stderr,
          "[ ERROR ] Cannot configure the RAM length:\n"
          "          %s.\n",
          strerror(errno)
        );

        _ram_dtor();
        return NULL;
      }

      if ( len_num < SEDNA_RAM_MIN_LEN ) {
        fprintf(
          stderr,
          "[ ERROR ] Cannot configure the RAM length:\n"
          "          too small (minimum: %" PRIu64 " ).\n",
          SEDNA_RAM_MIN_LEN
        );

        _ram_dtor();
        return NULL;
      }

      if ( SEDNA_RAM_MAX_LEN < len_num ) {
        fprintf(
          stderr,
          "[ ERROR ] Cannot configure the RAM length:\n"
          "          too large (maximum: %" PRIu64 ").\n",
          SEDNA_RAM_MAX_LEN
        );

        _ram_dtor();
        return NULL;
      }

      if ( len_num & 0x1 ) {
        fprintf(
          stderr,
          "[ ALERT ] Undefined RAM behaviour:\n"
          "          RAM length should be a power of 2.\n"
        );
      }

      ram->len = (u_word_t)len_num;
    }
  }

  ram->buf = (u_byte_t *)sedna_alloc(
    ram->len * sizeof(u_byte_t)
  );

  if ( !ram->buf ) {
    fprintf(
      stderr,
      "[ ERROR ] Cannot configure the RAM buffer:\n"
      "          %s.\n",
      strerror(errno)
    );

    _ram_dtor();
    return NULL;
  }

  return ram;
}

__SEDNA_PUBLIC struct sedna_ram_t * sedna_ram_dtor (
  __INOUT struct sedna_ram_t * ram
)
{
  if ( ram->buf ) {
    ram->buf = (u_byte_t *)sedna_dealloc(ram->buf);
    ram->len = U_WORD(0);
  }

  _ram_dtor();
  return ram;
}

__SEDNA_PUBLIC void sedna_ram_reset (
  __INOUT struct sedna_ram_t * ram,
  __IN    int                  lvl
)
{
  ram->cr = U_WORD(0);
  ram->ar = U_WORD(0);
  ram->dr = U_WORD(0);

  memset(ram->buf, 0, ram->len);
}

__SEDNA_PUBLIC void sedna_ram_clock (
  __INOUT struct sedna_ram_t * ram,
  __IN    int                  lvl
)
{
  if ( !( ram->cr & SEDNA_RAM_CRF_CS ) )
    return;

  if ( !( ram->cr & SEDNA_RAM_CRF_RQ ) )
    return;

  ram->ar &= ram->len - 1;
  
  if ( ram->cr & SEDNA_RAM_CRF_RD ) {
    ram->dr = ram->buf[ ram->ar ];
    ram->cr &= ~SEDNA_RAM_CRF_RQ;
    ram->cr |=  SEDNA_RAM_CRF_AC;
  }

  if ( ram->cr & SEDNA_RAM_CRF_WR ) {
    ram->buf[ ram->ar ] = ram->dr;
    ram->cr &= ~SEDNA_RAM_CRF_RQ;
    ram->cr |=  SEDNA_RAM_CRF_AC;
  }
}

__SEDNA_PUBLIC int sedna_ram_req (
  __INOUT struct sedna_ram_t * ram,
  __IN    u_word_t             cr,
  __IN    u_word_t             ar,
  __IN    u_word_t             dr
)
{
  if ( ram->cr & SEDNA_RAM_CRF_CS )
    return SEDNA_FAILURE;

  if ( ram->cr & SEDNA_RAM_CRF_RQ ) {
    if ( ram->ar != ar )
      return SEDNA_FAILURE;

    return SEDNA_WAITING;
  }

  if ( ram->cr & SEDNA_RAM_CRF_AC ) {
    if ( ram->ar != ar )
      return SEDNA_FAILURE;

    return SEDNA_SUCCESS;
  }

  ram->cr &= ~(
    SEDNA_RAM_CRF_RD |
    SEDNA_RAM_CRF_WR
  );

  ram->cr |= (
    cr & (
      SEDNA_RAM_CRF_RD |
      SEDNA_RAM_CRF_WR
    )
  ) | (
    SEDNA_RAM_CRF_CS |
    SEDNA_RAM_CRF_RQ
  );

  ram->ar = ar;
  ram->dr = dr;

  return SEDNA_WAITING;
}

__SEDNA_PUBLIC int sedna_ram_ack (
  __INOUT struct sedna_ram_t * ram,
  __IN    u_word_t             cr,
  __IN    u_word_t             ar,
  __OUT   u_word_t *           dr
)
{
  if ( !( ram->cr & SEDNA_RAM_CRF_CS ) )
    return SEDNA_FAILURE;

  if ( ram->cr & SEDNA_RAM_CRF_RQ ) {
    if ( ram->ar != ar )
      return SEDNA_FAILURE;

    return SEDNA_WAITING;
  }

  if ( ram->cr & SEDNA_RAM_CRF_AC ) {
    if ( ram->ar != ar )
      return SEDNA_FAILURE;

    if ( dr ) {
      int      shf = (int)( ar & 0x3 );
      u_word_t msk = U_WORD(0xFF);

      *dr &= ~( msk << shf );
      *dr |= ( ram->dr & msk ) << shf;
    }

    ram->cr &= ~(
      SEDNA_RAM_CRF_CS |
      SEDNA_RAM_CRF_AC
    );

    return SEDNA_SUCCESS;
  }

  return SEDNA_FAILURE;
}
