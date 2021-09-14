project('pthread_key', 'c')

pthread = dependency('threads')
dpdk = dependency('libdpdk')

executable('basic', 'basic.c',
    dependencies: pthread,
)

executable('dpdk', 'dpdk.c',
    dependencies: [pthread, dpdk],
)
