pkg = import('pkgconfig')
pkg_extra_cflags = ['-include', 'config.h'] + machine_args

pkg.generate(name: 'complicated_project',
	filebase: 'libcomplicated_project',
	version: meson.project_version(),
	description: '''Meson Complicated Project.''',
    subdirs: ['.'],
	extra_cflags: pkg_extra_cflags,
	libraries: ['-Wl,--as-needed'] + libraries,
	libraries_private: extra_ldflags)
