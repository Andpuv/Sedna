# ifndef __SEDNA_RAM_H
#   define __SEDNA_RAM_H

#   define SEDNA_RAM_MIN_LEN Ki(0x04) /*  4Ki */
#   define SEDNA_RAM_MAX_LEN Gi(0x04) /*  4Gi */
#   define SEDNA_RAM_DEF_LEN Ki(0x20) /* 32Ki */

#   define SEDNA_RAM_CRS_CS 0
#   define SEDNA_RAM_CRS_RD 1
#   define SEDNA_RAM_CRS_WR 2
#   define SEDNA_RAM_CRS_RQ 3
#   define SEDNA_RAM_CRS_AC 4

#   define SEDNA_RAM_CRM_CS U_WORD(0x1)
#   define SEDNA_RAM_CRM_RD U_WORD(0x1)
#   define SEDNA_RAM_CRM_WR U_WORD(0x1)
#   define SEDNA_RAM_CRM_RQ U_WORD(0x1)
#   define SEDNA_RAM_CRM_AC U_WORD(0x1)

#   define SEDNA_RAM_CRF_CS ( SEDNA_RAM_CRM_CS << SEDNA_RAM_CRS_CS )
#   define SEDNA_RAM_CRF_RD ( SEDNA_RAM_CRM_RD << SEDNA_RAM_CRS_RD )
#   define SEDNA_RAM_CRF_WR ( SEDNA_RAM_CRM_WR << SEDNA_RAM_CRS_WR )
#   define SEDNA_RAM_CRF_RQ ( SEDNA_RAM_CRM_RQ << SEDNA_RAM_CRS_RQ )
#   define SEDNA_RAM_CRF_AC ( SEDNA_RAM_CRM_AC << SEDNA_RAM_CRS_AC )

struct sedna_ram_t {
  struct sedna_obj_t obj;
  u_word_t           cr;
  u_word_t           ar;
  u_word_t           dr;
  u_word_t           len;
  u_byte_t *         buf;
};

__SEDNA_PUBLIC struct sedna_ram_t * sedna_ram_ctor (
  __INOUT struct sedna_ram_t * ram,
  __IN    struct sedna_cfg_t * cfg
);

__SEDNA_PUBLIC struct sedna_ram_t * sedna_ram_dtor (
  __INOUT struct sedna_ram_t * ram
);

__SEDNA_PUBLIC void sedna_ram_reset (
  __INOUT struct sedna_ram_t * ram,
  __IN    int                  lvl
);

__SEDNA_PUBLIC void sedna_ram_clock (
  __INOUT struct sedna_ram_t * ram,
  __IN    int                  lvl
);

__SEDNA_PUBLIC int sedna_ram_req (
  __INOUT struct sedna_ram_t * ram,
  __IN    u_word_t             cr,
  __IN    u_word_t             ar,
  __IN    u_word_t             dr
);

__SEDNA_PUBLIC int sedna_ram_ack (
  __INOUT struct sedna_ram_t * ram,
  __IN    u_word_t             cr,
  __IN    u_word_t             ar,
  __OUT   u_word_t *           dr
);

# endif
