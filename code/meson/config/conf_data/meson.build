project('conf_data', language: 'c')

conf = configuration_data()

conf.set('FOO', 123)
message('conf \'FOO\': @0@'.format(conf.get('FOO')))

arch = ''
cpu_family = host_machine.cpu_family()
if cpu_family.startswith('x86')
    arch = 'x86'
elif cpu_family.startswith('arm')
    arch = 'arm'
else
    error('unsupported CPU type @0@'.format(cpu_family))
endif
conf.set('arch', arch)
message('conf \'arch\': @0@'.format(conf.get('arch')))
