project('multi_libs_perf', 'c')

inc = [include_directories('.')]

subdir('libfoo')
executable('pref', 'main.c',
    include_directories: inc,
    link_args: '-ldl',
)
