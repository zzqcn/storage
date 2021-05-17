project('gen_reg', 'c')

subdir('tools')

sources = files(
    'main.c',
)

subdir('plugins')

message('generate register code for plugins...')
ret = run_command(make_regs,
    '@0@/plugins'.format(meson.current_source_dir()),
    '@0@/plugin_reg.c'.format(meson.current_source_dir())
).returncode()
if ret != 0
    error('generate register code failed')
endif
sources += '@0@/plugin_reg.c'.format(meson.current_source_dir())

executable('gen_reg', sources)