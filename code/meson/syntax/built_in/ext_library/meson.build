# find_library() 在 0.31.0 版本中废除
# lib = find_library('libpcap')
lib = meson.get_compiler('c').find_library('libpcap')

message('ext_library found(libpcap):' + lib.found().to_string())
if lib.found()
    message('ext_library.type_name():' + lib.type_name())
endif