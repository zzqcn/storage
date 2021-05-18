project('libfoo', 'c')

inc = include_directories('.')
libfoo = shared_library('foo', 'foo.c',
    include_directories : inc,
    install : true
)

libfoo_dep = declare_dependency(
    include_directories : inc,
    link_with : libfoo
)
