conf.set('ARCH_ARM', 1)

if conf.get('ARCH_32')
    conf.set('CACHE_LINE_SIZE', 64)
    conf.set('ARCH_ARMv7', 1)
    machine_args += '-mfpu=neon'
else
    conf.set('CACHE_LINE_SIZE', 128)
    conf.set('ARCH_ARM64', 1)
    # todo
endif
message(machine_args)

if (cc.get_define('__ARM_NEON', args: machine_args) != '' or
    cc.get_define('__aarch64__', args: machine_args) != '')
    compile_time_cpuflags += ['CPUFLAG_NEON']
endif

if cc.get_define('__ARM_FEATURE_CRC32', args: machine_args) != ''
    compile_time_cpuflags += ['CPUFLAG_CRC32']
endif

if cc.get_define('__ARM_FEATURE_CRYPTO', args: machine_args) != ''
    compile_time_cpuflags += ['CPUFLAG_AES', 'CPUFLAG_PMULL',
    'CPUFLAG_SHA1', 'CPUFLAG_SHA2']
endif
