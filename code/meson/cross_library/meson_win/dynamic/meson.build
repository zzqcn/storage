project('dynamic library', 'c')

dlib = shared_library('dlib', 'dlib.c',
  vs_module_defs: 'dlib.def',
)

executable('app', 'app.c',
  link_with : dlib,
)

executable('app2', 'app2.c')
