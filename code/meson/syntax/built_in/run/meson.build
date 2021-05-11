cc = meson.get_compiler('c')

code = '''
    #include <stdio.h>
    int main(void) { printf("hello meson"); return 123; }
'''
message('run(code).compiled():' + cc.run(code).compiled().to_string())
message('run(code).returncode():' + cc.run(code).returncode().to_string())

message('run(cmd).stdout():' + run_command('echo', 'Meson').stdout())
message('run(cmd).stderr():' + run_command('sh', 'NotExist').stderr())