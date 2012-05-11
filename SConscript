import os

build_dir = 'build'
proto_dir = 'proto'

environment = Environment(
  CCFLAGS = ['-Wall', '-pipe', '-std=c++0x', '-pthread'],
  LIBS = ['-lpthread'],
  ENV = os.environ,
  CPPPATH = ['.'],
)

# Configure different build modes.
if ARGUMENTS.get('debug') == '0' or ARGUMENTS.get('release') == '1':
	environment.Append(CCFLAGS = ['-O3'])
	environment.Append(CCFLAGS = ['-DNDEBUG'])
	environment.Append(CCFLAGS = ['-march=native'])
else:
	environment.Append(CCFLAGS = ['-g'])
	if ARGUMENTS.get('profile') == '1':
		environment.Append(CCFLAGS = ['-pg'])
		environment.Append(LINKFLAGS = ['-pg'])

#environment.Replace(CXX = "clang")

# Add support for glog.
environment.ParseConfig('pkg-config --cflags --libs libglog')
environment.ParseConfig('pkg-config --cflags --libs libgflags')
environment.ParseConfig('pkg-config --cflags --libs protobuf')

# Build protos.
proto_sources = [str(s) for s in Glob(os.path.join(proto_dir, '*.proto'))]
proto_sources.extend([str(s) for s in Glob(os.path.join(proto_dir, 'config/*.proto'))])
proto_sources.extend([str(s) for s in Glob(os.path.join(proto_dir, 'scene/*.proto'))])
proto_sources.extend([str(s) for s in Glob(os.path.join(proto_dir, 'util/*.proto'))])
source_target = [(s,  os.path.splitext(s)[0] + '.pb.h', os.path.splitext(s)[0] + '.pb.cc') for s in proto_sources]

for st in source_target:
  pb = environment.Command(
    [st[1], st[2]],
    st[0],
    'protoc --cpp_out=' + build_dir + ' ' + st[0]
  )

# Build binaries.
p = environment.Program(
  'raytracer',
   Glob('*.cc') 
    + Glob('listener/*.cc')
    + Glob('parser/*.cc')
    + Glob('renderer/*.cc')
    + Glob('renderer/sampler/*.cc')
    + Glob('renderer/shader/*.cc')
    + Glob('scene/*.cc') 
    + Glob('scene/geometry/*.cc')
    + Glob('util/*.cc')
    + [st[2] for st in source_target],
)

# This is how to force dependencies.
# environment.Depends(p, pb)
