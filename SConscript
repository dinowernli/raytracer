import os

build_dir = 'build'
proto_dir = 'proto'

environment = Environment(
  CCFLAGS = ['-Wall', '-pipe', '-std=c++0x', '-pthread'],
  LIBS = ['-lprotobuf', '-lz', '-lpthread'],
  ENV = os.environ,
  CPPPATH = ['.'],
  tools = ['default'],
)

# Build protos.
proto_sources = [str(s) for s in Glob(os.path.join(proto_dir, '*.proto'))]
source_target = [(s,  os.path.splitext(s)[0] + '.pb.h', os.path.splitext(s)[0] + '.pb.cc') for s in proto_sources]

for st in source_target:
  pb = environment.Command(
    [st[1], st[2]],
    st[0],
    'protoc --cpp_out=' + build_dir + ' ' + st[0]
  )

# Configure different build modes.
if ARGUMENTS.get('debug') == '0' or ARGUMENTS.get('release') == '1':
	environment.Append(CCFLAGS = ['-O2'])
else:
	environment.Append(CCFLAGS = ['-g'])
	if ARGUMENTS.get('profile') == '1':
		environment.Append(CCFLAGS = ['-pg'])
		environment.Append(LINKFLAGS = ['-pg'])

# Build binaries.
p = environment.Program(
  'Raytracer', 
   Glob('*.cc') 
    + Glob('exporter/*.cc')
    + Glob('renderer/*.cc')
	+ Glob('renderer/sampler/*.cc')
	+ Glob('renderer/shader/*.cc')
    + Glob('scene/*.cc') 
    + Glob('util/*.cc')
    + [st[2] for st in source_target],
)

# This is how to force dependencies.
# environment.Depends(p, pb)
