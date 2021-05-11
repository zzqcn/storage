e = environment()

# 0.58.0+版本中 MY_PATH 最终为 '/uar/local:/path1:/path2:/path3',
# 而在之前版本中为 '/usr/local', 因为之前版本不支持多次追加, 以最后一次为准
e.set('MY_PATH', '/path1')
e.append('MY_PATH', '/path2')
e.append('MY_PATH', '/path3')
e.prepend('MY_PATH', '/usr/local')

message('env MY_PATH:' +
    run_command('print.sh', env: e).stdout()
)