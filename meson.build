project('Sedna', 'c',
  version         : '0.0',
  default_options : [ 'warning_level=3' ]
)

sources = [
  'src/sedna-def.c',
  'src/sedna-linux.c',
  'src/sedna-cfg.c',
  'src/sedna-obj.c',
  'src/sedna.c'
]

headers = [
  'src/sedna-ver.h',
  'src/sedna-def.h',
  'src/sedna-linux.h',
  'src/sedna-cfg.h',
  'src/sedna-obj.h',
  'src/sedna.h'
]

headers_dir = include_directories('src')

library_dir = join_paths(get_option('prefix'), 'lib', 'sedna')
binary_dir  = join_paths(get_option('prefix'), 'bin', 'sedna')

if 'static' == get_option('type')
  sedna_lib = static_library('sedna', sources,
    include_directories : headers_dir,
    install             : true,
    install_dir         : library_dir
  )
else
  sedna_lib = shared_library('sedna', sources,
    include_directories : headers_dir,
    install             : true,
    install_dir         : library_dir
  )
endif

install_headers(headers,
  install_dir : join_paths(get_option('prefix'), 'include', 'sedna')
)

sedna_exe = executable('sedna-test', 'src/main.c',
  include_directories : headers_dir,
  link_with           : sedna_lib,
  install             : true,
  install_dir         : binary_dir,
  install_rpath       : library_dir
)
