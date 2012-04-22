import os

environment = Environment(
  CCFLAGS = ['-Wall', '-pipe', '-std=c++0x'], 
  ENV = os.environ,
  CPPPATH = ['.']
)

# Build modes
if ARGUMENTS.get('debug') == '0' or ARGUMENTS.get('release') == '1':
	environment.Append(CCFLAGS = ['-O2'])
else:
	environment.Append(CCFLAGS = ['-g'])
	if ARGUMENTS.get('profile') == '1':
		environment.Append(CCFLAGS = ['-pg'])
		environment.Append(LINKFLAGS = ['-pg'])

environment.Program('Raytracer', source = Glob('*.cc'))
