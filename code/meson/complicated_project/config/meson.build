machine_args = []
ext_args = []

supported_exec_envs = ['linux', 'darwin']
exec_env = host_machine.system()
if not supported_exec_envs.contains(exec_env)
    error('unsupported system type @0@'.format(exec_env))
endif

foreach env : supported_exec_envs
    set_variable('is_' + env, exec_env == env)
    conf.set('OS_' + exec_env.to_upper(), 1)
endforeach

# set the major version, which might be used by drivers and libraries
# depending on the configuration options
version = meson.project_version().split('.')
major_version = '@0@.@1@'.format(version.get(0), version.get(1))
so_version = version.get(0)

# extract all version information into the build configuration
conf.set('VER_MAJOR', version.get(0).to_int())
conf.set('VER_MINOR', version.get(1).to_int())
conf.set('VER_PATCH', version.get(2).to_int())
conf.set_quoted('VER_STRING', meson.project_version())

# set the machine type and cflags for it
if meson.is_cross_build()
    machine = host_machine.cpu()
else
    machine = get_option('machine')
endif

# machine type 'default' is special, it defaults to the per arch agreed common
# minimal baseline needed for DPDK.
# That might not be the most optimized, but the most portable version while
# still being able to support the CPU features required for DPDK.
# This can be bumped up by the DPDK project, but it can never be an
# invariant like 'native'
if machine == 'default'
    if host_machine.cpu_family().startswith('x86')
        # matches the old pre-meson build systems default
        machine = 'corei7'
    elif host_machine.cpu_family().startswith('arm')
        machine = 'armv7-a'
    elif host_machine.cpu_family().startswith('aarch')
        # arm64 manages defaults in config/arm/meson.build
        machine = 'default'
    endif
endif

# ppc64 does not support -march= at all, use -mcpu and -mtune for that
if host_machine.cpu_family().startswith('ppc')
    machine_args += '-mcpu=' + machine
    machine_args += '-mtune=' + machine
else
    machine_args += '-march=' + machine
endif

toolchain = cc.get_id()
# https://stackoverflow.com/questions/63665653/different-behavior-between-clang-and-gcc-10-when-linking-to-static-library-conta
if(toolchain == 'clang')
    ext_args += '-fno-common'
endif

conf.set('MACHINE', machine)
conf.set('ARCH_64', cc.sizeof('void *') == 8)
conf.set('ARCH_32', cc.sizeof('void *') == 4)
conf.set_quoted('TOOLCHAIN', toolchain)
conf.set('TOOLCHAIN_' + toolchain.to_upper(), 1)

if get_option('enable_XXX')
    conf.set('ENABLE_XXX', 1)
endif

if get_option('tests')
    ext_args += '-DTEST'
endif

compile_time_cpuflags = []

subdir(arch_subdir)

# add -include config.h to cflags
add_project_arguments('-include', 'config.h', language: 'c')
