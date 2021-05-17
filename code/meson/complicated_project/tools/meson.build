echo_tool = find_program('echo.sh')

python3 = import('python').find_installation(required: false)
if python3.found()
	py3 = [python3]
else
	py3 = ['meson', 'runpython']
endif
sphinx_wrapper = py3 + files('sphinx_caller.py')
