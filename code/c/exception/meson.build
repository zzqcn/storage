project('c_exception', 'c')

cc = meson.get_compiler('c')

if cc.get_id() == 'msvc'
    add_project_arguments(cc.get_supported_arguments(['/utf-8']), language: 'c')
endif

executable('jmp', 'jmp.c')
executable('jmp2', 'jmp2.c')
executable('jmp3', 'jmp3.c')
executable('simple_exception', 'simple_exception.c')
executable('simple_exception2', 'simple_exception2.c')
executable('simple_exception3', 'simple_exception3.c')
