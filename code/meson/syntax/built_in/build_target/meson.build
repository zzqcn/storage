target = executable('hello', 'main.c')

message('build_target.name():' + target.name())
message('build_target.full_path():' + target.full_path())
objs = [target.extract_all_objects()]
message('build_target.extract_all_objects():' + objs.length().to_string())
foreach o : objs
    message('obj: @0@'.format(o))
endforeach
