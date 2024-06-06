# include <stdlib.h>
# include "sedna.h"

int main ( int argc, char ** argv )
{
  if ( !argc )
    return EXIT_FAILURE;

  { /* example */
    sedna_obj_tracking_open();
    do {
      struct sedna_cfg_t ram_cfg;

      if ( sedna_cfg_ctor(&ram_cfg, "--len 8K") ) {
        fprintf(stderr, "[ PANIC ] Cannot create RAM configuration.\n");
        break;
      }

      struct sedna_ram_t * ram = sedna_ram_ctor(NULL, &ram_cfg);

      if ( !ram ) {
        fprintf(stderr, "[ PANIC ] Cannot create RAM.\n");
      } else {
        fprintf(stderr, "[  INFO ] RAM is %u bytes long.\n", ram->len);
      }

      ram = sedna_ram_dtor(ram); /* should not cause free() abortion */
      sedna_obj_tracking_dump(stdout);
    } while (false);
    sedna_obj_tracking_close();
  }

  for (;;);

  return EXIT_SUCCESS;
}
