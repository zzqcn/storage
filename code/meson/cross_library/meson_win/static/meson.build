project('static library', 'c')

slib = static_library('slib', 'slib.c')
executable('app', 'app.c',
  link_with : slib,
)
