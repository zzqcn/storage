headers += files(
    'complicated.h',
)

complicated_includes = include_directories('.')
complicated_libs = [baselib]

complicated = shared_library('complicated', 'complicated.c',
    c_args: global_cflags,
    include_directories: global_includes,
    link_with: complicated_libs,
    version: major_version,
    soversion: so_version,
    install: true,
)