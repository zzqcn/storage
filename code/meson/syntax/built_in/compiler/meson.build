cc = meson.get_compiler('c')

message('compiler.get_id():' + cc.get_id())
message('compiler.version():' + cc.version())

define = '__linux__'
message('compiler.get_define(__linux__):' + cc.get_define(define))
message('compiler.get_linker_id():' + cc.get_linker_id())
header = 'stdio.h'
message('compiler.has_header(stdio.h):' + cc.has_header(header).to_string())
function = 'printf'
message('compiler.has_function(printf):' + cc.has_function(function).to_string())
library = 'libpcap'
message('compiler.find_library(libpcap):' + cc.find_library(library).found().to_string())

code = '''
    #include <stdio.h>
    int main(void) { printf("hello meson"); return 0; }
'''
message('compiler.compiles(code):' + cc.compiles(code).to_string())
message('compiler.links(code):' + cc.links(code).to_string())
message('compiler.run(code):' + cc.run(code).returncode().to_string())
