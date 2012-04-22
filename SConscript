import os

build_dir = 'build'
proto_dir = 'proto'

environment = Environment(
  CCFLAGS = ['-Wall', '-pipe', '-std=c++0x'], 
  ENV = os.environ,
  CPPPATH = ['.'],
  tools = ['default', 'protoc'],
  toolpath = ['.']
)

proto_files = environment.Protoc(
    [],
    Glob(os.path.join(proto_dir, '*.proto')),
    PROTOCPROTOPATH=['.'],
    PROTOCPYTHONOUTDIR=None,
    PROTOCOUTDIR = build_dir,
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
