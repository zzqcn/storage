# we require SSE4.2 for DPDK
if cc.get_define('__SSE4_2__', args: machine_args) == ''
	message('SSE 4.2 not enabled by default, explicitly enabling')
	machine_args += '-msse4'
endif

base_flags = ['SSE', 'SSE2', 'SSE3','SSSE3', 'SSE4_1', 'SSE4_2']
foreach f:base_flags
	compile_time_cpuflags += ['CPUFLAG_' + f]
endforeach

optional_flags = [
	'AES',
	'AVX',
	'AVX2',
	'AVX512BW',
	'AVX512CD',
	'AVX512DQ',
	'AVX512F',
	'AVX512VL',
	'PCLMUL',
	'RDRND',
	'RDSEED',
	'VPCLMULQDQ',
]
foreach f:optional_flags
	if cc.get_define('__@0@__'.format(f), args: machine_args) == '1'
		if f == 'PCLMUL' # special case flags with different defines
			f = 'PCLMULQDQ'
		elif f == 'RDRND'
			f = 'RDRAND'
		endif
		compile_time_cpuflags += ['CPUFLAG_' + f]
	endif
endforeach

conf.set('ARCH_X86', 1)
if conf.get('ARCH_64')
	conf.set('ARCH_X86_64', 1)
	conf.set('ARCH', 'x86_64')
else
	conf.set('ARCH_I686', 1)
	conf.set('ARCH', 'i686')
endif

conf.set('CACHE_LINE_SIZE', 64)
