project('sedna', 'c',
  version         : '0.0',
  default_options : ['warning_level=3']
)

sources = [
  'src/sedna-com.c',
  'src/sedna-linux.c',
  'src/sedna-cfg.c',
  'src/sedna-obj.c',
  'src/sedna.c'
]

headers = [
  'src/sedna-ver.h',
  'src/sedna-linux.h',
  'src/sedna-com.h',
  'src/sedna-cfg.h',
  'src/sedna-obj.h',
  'src/sedna.h'
]

headers_dir = include_directories('src')

if 'static' == get_option('type')
  sedna_lib = static_library('sedna', sources,
    include_directories : headers_dir,
    install             : true
  )
else
  sedna_lib = shared_library('sedna', sources,
    include_directories : headers_dir,
    install             : true
  )
endif

install_headers(headers)

sedna_exe = executable('sedna-test', 'src/main.c',
  include_directories : headers_dir,
  link_with           : sedna_lib
)
