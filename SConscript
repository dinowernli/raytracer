import os

#build_dir = 'build'
proto_dir = 'proto'

environment = Environment(
  CCFLAGS = ['-Wall', '-pipe', '-std=c++0x', '-pthread'],
  LIBS = ['-lprotobuf', '-lz', '-lpthread'],
  ENV = os.environ,
  CPPPATH = ['.'],
  tools = ['default', 'protoc'],
  toolpath = ['.']
)

pb = environment.Command(
  ['proto/RaytracerConfig.pb.h', 'proto/RaytracerConfig.pb.cc'],
  'proto/RaytracerConfig.proto',
  'protoc --cpp_out=build/ proto/RaytracerConfig.proto'
)

#proto_files = environment.Protoc(
#  [],
#  Glob(os.path.join(proto_dir, '*.proto')),
#  PROTOCPROTOPATH=['.'],
#  PROTOCPYTHONOUTDIR=None,
#  #PROTOCOUTDIR = build_dir,
  #PROTOCOUTDIR = '.',
#  #PROTOCOUTDIR = proto_dir,
#)

# Build modes
if ARGUMENTS.get('debug') == '0' or ARGUMENTS.get('release') == '1':
	environment.Append(CCFLAGS = ['-O2'])
else:
	environment.Append(CCFLAGS = ['-g'])
	if ARGUMENTS.get('profile') == '1':
		environment.Append(CCFLAGS = ['-pg'])
		environment.Append(LINKFLAGS = ['-pg'])

p = environment.Program(
  'Raytracer', 
  [
   #proto_files[0], 
   #proto_files[1],
   'proto/RaytracerConfig.pb.cc',
   Glob('*.cc'),
  ]
)

#environment.Depends(p, pb)
