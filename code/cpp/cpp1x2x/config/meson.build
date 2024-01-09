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
conf.set_quoted('SYSTEM_NAME', host_os_name)

# 编译器
cxx_name = cxx.get_id()
supported_cxxs = ['gcc', 'clang', 'msvc']
if not supported_cxxs.contains(cxx_name)
    error('unsupported C++ Compiler type @0@'.format(cxx_name))
endif
conf.set(cxx_name.to_upper(), 1)
conf.set('CXX_NAME', '"@0@ @1@"'.format(cxx_name, cxx.version()))

# C++标准
cpp_std_name = get_option('cpp_std')
conf.set_quoted('CPP_STD_NAME', cpp_std_name)

# Boost库
boost_version = boost.version()
conf.set_quoted('DEP_BOOST_VERSION', boost_version)

# add -include config.h to cflags
if cxx_name != 'msvc'
    add_project_arguments('-include', 'config.h', language: 'cpp')
    # 对于clang 15, 开启std::views支持
    if cxx_name == 'clang'
        add_project_arguments('-fexperimental-library', language: 'cpp')
    endif
else
    add_project_arguments('/FI', 'config.h', language: 'cpp')
endif
