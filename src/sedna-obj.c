# include "sedna.h"

# ifdef __SEDNA_REQUIRES_TRACKING

__SEDNA_PRIVATE void _obj_tracking_attach (
  __IN    struct sedna_obj_t * obj
);

__SEDNA_PRIVATE void _obj_tracking_detach (
  __IN    struct sedna_obj_t * obj
);

# else
#   define _obj_tracking_attach(obj)
#   define _obj_tracking_detach(obj)
# endif

__SEDNA_PUBLIC struct sedna_obj_t * sedna_obj_ctor (
  __INOUT struct sedna_obj_t * obj,
  __IN    size_t               len
)
{
  if ( len < sizeof(struct sedna_obj_t) ) {
    len = sizeof(struct sedna_obj_t);
  }

  if ( !obj ) {
    obj = (struct sedna_obj_t *)sedna_alloc(len);

    if ( obj ) {
      obj->in_heap = true;
    }
  } else {
    obj->in_heap = false;
  }

  _obj_tracking_attach(obj);

  return obj;
}

__SEDNA_PUBLIC struct sedna_obj_t * sedna_obj_dtor (
  __INOUT struct sedna_obj_t * obj
)
{
  if ( !obj->in_heap )
    return obj;

  _obj_tracking_detach(obj);

  return (struct sedna_obj_t *)sedna_dealloc(obj);
}

# ifdef __SEDNA_REQUIRES_TRACKING

__SEDNA_PRIVATE int _objc = 0;
__SEDNA_PRIVATE struct sedna_obj_t * _objh;
__SEDNA_PRIVATE struct sedna_obj_t * _objt;

__SEDNA_PUBLIC int sedna_obj_tracking_open (
)
{
# ifdef __SEDNA_REQUIRES_VERBOSITY
  if ( _objc ) {
    fprintf(
      stderr,
      "[ ERROR ] Object tracking already started.\n"
    );

    return SEDNA_FAILURE;
  }
# else
  if ( _objc )
    return SEDNA_FAILURE;
# endif

  _objh = (struct sedna_obj_t *)NULL;
  _objt = (struct sedna_obj_t *)NULL;

# ifdef __SEDNA_REQUIRES_VERBOSITY
  fprintf(
    stderr,
    "[  INFO ] Object tracking started.\n"
  );
# endif

  return SEDNA_SUCCESS;
}

__SEDNA_PUBLIC int sedna_obj_tracking_dump (
  __IN    FILE * fp
)
{
  if ( !_objc )
    return 0;

  struct sedna_obj_t * cur = _objh;

  int res = fprintf(fp, "%d objects: ", _objc);

  if ( res < 0 )
    return res;

  while ( cur ) {
    int n;

    if ( !cur->next ) {
      n = fprintf(fp, "%p", cur);
    } else {
      n = fprintf(fp, "%p > ", cur);
    }

    if ( n < 0 )
      return -res;

    res += n;
    cur = cur->next;
  }

  int n = fprintf(fp, "\n");

  if ( n < 0 )
    return -res;

  return res + n;
}

__SEDNA_PUBLIC int sedna_obj_tracking_close (
)
{
# ifdef __SEDNA_REQUIRES_VERBOSITY
  fprintf(
    stderr,
    "[  INFO ] Object tracking terminated with %d objects.\n",
    _objc
  );
# endif

  struct sedna_obj_t * cur = _objh;
  int cnt = 0;

  while ( cur ) {
    struct sedna_obj_t * obj = cur;
    cur = cur->next;

    _obj_tracking_detach(obj);

# ifdef __SEDNA_REQUIRES_VERBOSITY
    fprintf(
      stderr,
      "[  INFO ] Object %p detached.\n",
      obj
    );
# endif

    obj = sedna_obj_dtor(obj);

# ifdef __SEDNA_REQUIRES_VERBOSITY
    if ( obj ) {
      fprintf(
        stderr,
        "[ ALERT ] Floating object %p (memory leak?).\n",
        obj
      );
    } else {
      ++cnt;
    }
# else
    ++cnt;
# endif
  }

# ifdef __SEDNA_REQUIRES_VERBOSITY
  fprintf(
    stderr,
    "[  INFO ] Object tracking deallocated %d objects.\n",
    cnt
  );
# endif

  return _objc - cnt;
}

__SEDNA_PRIVATE void _obj_tracking_attach (
  __IN    struct sedna_obj_t * obj
)
{
  obj->next = (struct sedna_obj_t *)NULL;
  obj->prev = (struct sedna_obj_t *)NULL;

  if ( !_objt ) {
    _objh = obj;
    _objt = obj;
  } else {
    obj->prev   = _objt;
    _objt->next = obj;
    _objt       = obj;
  }

  ++_objc;
}

__SEDNA_PRIVATE void _obj_tracking_detach (
  __IN    struct sedna_obj_t * obj
)
{
  if ( obj->prev ) {
    obj->prev->next = obj->next;
  }

  if ( obj->next ) {
    obj->next->prev = obj->prev;
  }

  obj->next = (struct sedna_obj_t *)NULL;
  obj->prev = (struct sedna_obj_t *)NULL;
  --_objc;
}

# endif
