project('conf_gen', 'c')

conf = configuration_data()

conf.set('MAX_THREADS', get_option('max_threads'))
if get_option('with_dpdk')
    conf.set('WITH_DPDK', 1)
endif

configure_file(
    output: 'config.h',
    configuration: conf,
    install: true,
    install_dir: get_option('includedir')
)

executable('myapp', 'main.c')