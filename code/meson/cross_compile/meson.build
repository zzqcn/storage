project('cross_compile', 'c')

if meson.is_cross_build()
    message('cross build')
else
    message('native build')
endif
message('C compiler: ' + meson.get_compiler('c').get_id())
message('host_machine.cpu_family:' + host_machine.cpu_family())
message('host_machine.cpu():' + host_machine.cpu())
message('host_machine.system():' + host_machine.system())
message('host_machine.endian():' + host_machine.endian())

native_exe = executable('native_hello', 'main.c', native: true)
cross_exe = executable('cross_hello', 'main.c', native: false)

test('run native hello', native_exe)
test('run cross hello', cross_exe, env: ['QEMU_LD_PREFIX=/usr/arm-linux-gnueabi'])