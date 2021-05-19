project('gen_target', 'c')

sources = files(
    'cmd.c',
    'main.c',
)

# bison --defines=grammar.h -o grammar.c grammar.y
# flex -o lex.c lex.l

flex = find_program('flex', required: true)
bison = find_program('bison', required: true)

bison_out = custom_target('bison_out',
    output: ['@BASENAME@.h', '@BASENAME@.c'],
    input: 'grammar.y',
    command: [bison, '--defines=@OUTPUT0@', '-o', '@OUTPUT1@', '@INPUT@'],
)

flex_out = custom_target('flex_out',
    output: '@BASENAME@.c',
    input: 'lex.l',
    command: [flex, '-o', '@OUTPUT0@', '@INPUT@'],
)

sources += [bison_out, flex_out]

executable('gen_target', sources,
)
