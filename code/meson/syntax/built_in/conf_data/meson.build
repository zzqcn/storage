conf = configuration_data()

conf.set('FOO', 123)
conf.set('BAR1', true)
conf.set10('BAR2', true)
conf.set_quoted('BAZ', 'hello')

# keys方法从0.57.0才开始支持
if meson.version().version_compare('>=0.57')
    keys = conf.keys()
    foreach k : keys
        message('conf key:' + k)
    endforeach
else
    message('current meson @0@ version don\'t support keys() method'
        .format(meson.version()))
endif

message('conf.get(FOO):' + conf.get('FOO', 100).to_string())
message('conf.get(BAZ):' + conf.get_unquoted('BAZ'))
message('conf.get_unquoted(BAZ):' + conf.get_unquoted('BAZ'))
message('conf.has(BAR2):' + conf.get('BAR2').to_string())