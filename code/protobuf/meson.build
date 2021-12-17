project('pbc_demo', 'c', 'cpp',
  default_options: [
        'cpp_std=c++11',
    ]
)

add_project_arguments(
    '-Wfatal-errors',
    '-Wno-unused-function',
    language: ['c', 'cpp'],
)

libpb = dependency('protobuf', required: true)
libpbc = dependency('libprotobuf-c', required: true)
libpb2json = declare_dependency(
    include_directories: [
        '/usr/local/include',
    ],
    link_args: [
        '-L/usr/local/lib',
        '-lprotobuf2json-c',
    ]
)

executable('hellocpp', 'hello.cpp', 'hello.pb.cc',
  dependencies: [libpb]
)

executable('helloc', 'hello.c', 'hello.pb-c.c',
  dependencies: [libpbc, libpb2json]
)
