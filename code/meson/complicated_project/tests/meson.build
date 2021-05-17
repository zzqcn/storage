tests_inc = [global_includes, complicated_includes]

executable('test', 'test.c',
    c_args: global_cflags,
    include_directories: tests_inc,
    link_with: [complicated],
)