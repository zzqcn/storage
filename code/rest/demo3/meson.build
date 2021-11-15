project('complicated_project', 'c',
    version: run_command(find_program('cat', 'more'),
        files('VERSION')).stdout().strip(),
)

cc = meson.get_compiler('c')

subdir('tools')
subdir('app')
if get_option('docs')
    subdir('docs')
endif
