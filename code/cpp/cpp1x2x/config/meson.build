# 平台兼容性检查(CPU)
host_cpu_name = host_machine.cpu_family()
supported_cpus = ['x86', 'x86_64', 'arm', 'aarch64']
if not supported_cpus.contains(host_cpu_name)
    error('unsupported CPU type @0@'.format(host_cpu_name))
endif
conf.set(host_cpu_name.to_upper(), 1)
conf.set_quoted('CPU_NAME', host_cpu_name)

# 平台兼容性检查(操作系统)
host_os_name = host_machine.system()
supported_oss = ['linux', 'windows', 'darwin']
if not supported_oss.contains(host_os_name)
    error('unsupported System type @0@'.format(host_os_name))
endif
conf.set(host_os_name.to_upper(), 1)
conf.set_quoted('HOST_NAME', host_os_name)

# 编译器
toolchain = cxx.get_id()
supported_cxxs = ['gcc', 'clang', 'msvc']
if not supported_cxxs.contains(toolchain)
    error('unsupported Toolchain type @0@'.format(toolchain))
endif
conf.set(toolchain.to_upper(), 1)
conf.set('TOOLCHAIN_NAME', '"@0@ @1@"'.format(toolchain, cxx.version()))


# add -include config.h to cflags
if toolchain != 'msvc'
    add_project_arguments('-include', 'config.h', language: 'cpp')
else
    add_project_arguments('/FI', 'config.h', language: 'cpp')
endif
