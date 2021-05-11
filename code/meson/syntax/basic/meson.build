# variable
var1 = 123
# var2是var1的拷贝
var2 = var1
# 改变var2的值并不影响var1
var2 = 456
message('var1: @0@, var2: @1@'.format(var1, var2))
message('var1:' + get_variable('var1').to_string())
set_variable('var1', 'hello')
message('var1:' + var1)

# number
x = 1 + 2
y = 3 * 4
d = 5 % 3 # d结果为2
message('x: @0@, y: @1@, d: @2@'.format(x, y, d))
message('int to str: ' + y.to_string())
str = '123'
message('str to int: @0@'.format(str.to_int()))
x = 123
message('123 is_even: @0@, is_odd: @1@'.
    format(x.is_even(), x.is_odd())
)

# boolean
x = true
message('bool x: @0@'.format(x))
message('bool to int: @0@, bool to str: @1@'
    .format(x.to_int(), x.to_string())
)

# string
str1 = 'abc'
str2 = 'xyz'
combined = str1 + '_' + str2 # combined is now abc_xyz
message('combined str: ' + combined)

joined = '/usr/share' / 'projectname'    # => /usr/share/projectname
message('joned: ' + joined)
joined = '/usr/local' / '/etc/name'      # => /etc/name
message('joned: ' + joined)
joined = 'C:\\foo\\bar' / 'builddir'     # => C:/foo/bar/builddir
message('joned: ' + joined)
joined = 'C:\\foo\\bar' / 'D:\\builddir' # => D:/builddir
message('joned: ' + joined)

multiline_string = '''#include <foo.h>
int main (int argc, char ** argv) {
  return FOO_SUCCESS;
}'''
message('multiline_string: ' + multiline_string)

template = 'string: @0@, number: @1@, bool: @2@'
res = template.format('text', 1, true)
message('res: ' + res)

# array
arr = [1, 'string', true]
arr += ['foo']
arr += 2
message('arr length: @0@, arr contains foo: @1@'
    .format(arr.length(), arr.contains('foo'))
)
message('arr[2]: @0@, arr[-1]: @1@'.format(arr[2], arr[-1]))
message('arr[2]: @0@, arr[-1]: @1@'.format(arr.get(2), arr.get(-1)))
foreach i : arr
    message('@0@'.format(i))
endforeach

# dictionary
dict = {'foo': 123}
dict += {'bar': 'Meson'}
if 'foo' in dict
    message('foo in dict')
endif
if dict.has_key('bar')
    message('dict has key \'bar\', value: @0@'.format(dict.get('bar')))
endif
foreach k : dict.keys()
    message(k)
endforeach
foreach k,v : dict
    message('key: @0@, value: @1@'.format(k, v))
endforeach
