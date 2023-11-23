project('cpp1x2x', 'cpp',
    version : '0.1',
)

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

subdir('type')

sources += files(
    'cpp1x2x.cpp',
)

executable('cpp1x2x', sources,
    include_directories: includes,
    # cpp_args: cpp_args,
    # dependencies: deps,
    # link_with: libs,
    # link_args: link_args,
)
