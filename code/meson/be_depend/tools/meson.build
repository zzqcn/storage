pkg = import('pkgconfig')

pkg.generate(name: 'Foo',
	filebase: 'libfoo',
	version: '2.5',
	description: '''Foo Library.
This is a demo library for understanding meson dependency.''',
    subdirs: ['.'],
	libraries: ['-Wl,--as-needed'] + foo_libraries)