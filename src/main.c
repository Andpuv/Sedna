# include <stdlib.h>
# include "sedna.h"

int main ( int argc, char ** argv )
{
  if ( !argc )
    return EXIT_FAILURE;

  {
    sedna_obj_tracking_open();
    {
      struct sedna_obj_t * objs [ 4 ];

      objs[ 0 ] = sedna_obj_ctor(NULL, 0);
      objs[ 1 ] = sedna_obj_ctor(NULL, 0);
      objs[ 2 ] = sedna_obj_ctor(NULL, 0);
      objs[ 3 ] = sedna_obj_ctor(NULL, 0);

      sedna_obj_tracking_dump(stdout);

      objs[ 0 ] = sedna_obj_ctor(NULL, 0);
      objs[ 1 ] = sedna_obj_ctor(NULL, 0);
      objs[ 2 ] = sedna_obj_ctor(NULL, 0);
      objs[ 3 ] = sedna_obj_ctor(NULL, 0);

      sedna_obj_tracking_dump(stdout);
    }
    sedna_obj_tracking_close();
  }

  return EXIT_SUCCESS;
}
