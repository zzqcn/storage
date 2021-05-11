project('Foo Library', 'c')

foo_libraries = []
foo_libraries += shared_library('foo', 'foo_library.c',
    version: '2.5',
    soversion: '2',
    install: true,
)

foo_headers = files(
    'foo_library.h',
)
install_headers(foo_headers)

subdir('tools')
