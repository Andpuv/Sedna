# ifndef __SEDNA_OBJ_H
#   define __SEDNA_OBJ_H

struct sedna_obj_t {
#   ifdef __SEDNA_REQUIRES_TRACKING
  struct sedna_obj_t * next;
  struct sedna_obj_t * prev;
#   endif

  int in_heap;
};

__SEDNA_PUBLIC struct sedna_obj_t * sedna_obj_ctor (
  __INOUT struct sedna_obj_t * obj,
  __IN    size_t               len
);

__SEDNA_PUBLIC struct sedna_obj_t * sedna_obj_dtor (
  __INOUT struct sedna_obj_t * obj
);

#   ifdef __SEDNA_REQUIRES_TRACKING

__SEDNA_PUBLIC int sedna_obj_tracking_open (
);

__SEDNA_PUBLIC int sedna_obj_tracking_dump (
  __IN    FILE * fp
);

__SEDNA_PUBLIC int sedna_obj_tracking_close (
);

#   else
#     define sedna_obj_tracking_open()   ( SEDNA_SUCCESS )
#     define sedna_obj_tracking_dump(fp) ( 0 )
#     define sedna_obj_tracking_close()  ( SEDNA_SUCCESS )
#   endif

# endif
