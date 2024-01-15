project('cpp1x2x', 'cpp',
    version : '0.1',
    default_options: [
        'cpp_std=c++20',
    ],
)

# https://mesonbuild.com/Dependencies.html#boost
boost = dependency('boost', required: true)

# 变量声明
cxx = meson.get_compiler('cpp')
conf = configuration_data()
headers = []
libraries = []
extra_ldflags = []
cflags = []
includes = []
sources = []

subdir('config')

build_cfg = 'config.h'
configure_file(
    output: build_cfg,
    configuration: conf,
)

includes += include_directories(
    '.'
)

sources += files(
    'type.cpp',
    'decltype.cpp',
    'auto.cpp',
    'rvref.cpp',
    'ret_behind.cpp',
    'namespace.cpp',
    'range.cpp',
    'span.cpp',
    'string_view.cpp',
    'regex.cpp',
    'cpp1x2x.cpp',
)

subdir('smart_pointer')
subdir('variant_visit')
subdir('template')
subdir('concurrent')

executable('cpp1x2x', sources,
    include_directories: includes,
    # cpp_args: cpp_args,
    dependencies: [boost],
    # link_with: libs,
    # link_args: link_args,
)
