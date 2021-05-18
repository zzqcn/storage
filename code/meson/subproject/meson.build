project('foo_app', 'c')

#libfoo_dep = dependency('libfoo', required : false)
#
#if not libfoo_dep.found()
#    libfoo_proj = subproject('libfoo')
#    libfoo_dep = libfoo_proj.get_variable('libfoo_dep')
#endif

libfoo_dep = dependency('libfoo', fallback : ['libfoo', 'libfoo_dep'])

executable('foo', 'main.c',
    dependencies : libfoo_dep,
    install : true
)