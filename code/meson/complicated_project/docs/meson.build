sphinx = find_program('sphinx-build', required: get_option('docs'))
extra_sphinx_args = []

doc_target = custom_target('doc',
    input: files('index.rst'),
    output: 'dev_doc',
    command: [sphinx_wrapper, sphinx, meson.project_version(),
        meson.current_source_dir(), meson.current_build_dir(),
        extra_sphinx_args],
    depfile: '.html.d',
    build_by_default: get_option('docs'),
)

run_target('doc',
    command: ['echo', 'building develop docs...', doc_target]
)