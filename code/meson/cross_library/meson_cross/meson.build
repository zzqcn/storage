project('cross-platform library', 'c')

dlib_cflags = []
app3_lkflags = []

host_os_name = host_machine.system()
if host_os_name == 'windows'
  dlib_cflags += '-DLIB_EXPORTS'
else
  app3_lkflags += '-ldl'
endif

slib = static_library('slib', 'slib.c')

dlib = shared_library('dlib', 'dlib.c',
  c_args: dlib_cflags,
  vs_module_defs: 'dlib.def',
)

# link static library
executable('app', 'app.c',
  link_with : slib,
)

# link shared library at building
executable('app2', 'app2.c',
  link_with : dlib,
)

# load shared library at runtime
executable('app3', 'app3.c',
  link_args: app3_lkflags,
)
