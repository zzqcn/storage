project('complicated_project', 'c',
    version: run_command(find_program('cat', 'more'),
        files('VERSION')).stdout().strip(),
    default_options: [
        'buildtype=debug',
        'b_lundef=false',
        'includedir=include/complicated_project'
    ],
)

cc = meson.get_compiler('c')
conf = configuration_data()
headers = []
libraries = []
extra_ldflags = []
version_file = files('VERSION')
global_cflags = []

if get_option('buildtype').startswith('debug')
    add_project_arguments('-DDEBUG', language: 'c')
endif
add_project_arguments('-Wno-unused-function', language: 'c')

if host_machine.cpu_family().startswith('x86')
    arch_subdir = 'x86'
elif host_machine.cpu_family().startswith('arm') or
    host_machine.cpu_family().startswith('aarch')
    arch_subdir = 'arm'
else
    error('unsupported CPU type @0@'.format(host_machine.cpu_family()))
endif

subdir('config')
subdir('tools')

global_includes = include_directories(
    '.',
    'baselib',
)

global_cflags += machine_args
global_cflags += ext_args

build_cfg = 'config.h'
configure_file(
    output: build_cfg,
    configuration: conf,
    install: true,
    install_dir: get_option('includedir')
)

subdir('baselib')
subdir('complicated')
subdir('examples')
if get_option('tests')
    subdir('tests')
endif
if get_option('docs')
    subdir('docs')
endif

install_headers(headers)
subdir('tools/pkg_config')
