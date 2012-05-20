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

# Add support for the google libs.
environment.ParseConfig('pkg-config --cflags --libs libglog')
environment.ParseConfig('pkg-config --cflags --libs libgflags')
environment.ParseConfig('pkg-config --cflags --libs protobuf')

# Build protos.
proto_files = [
  '*.proto',
  'config/*.proto',
  'scene/*.proto',
  'util/*.proto',
]

proto_sources = []
for string in proto_files:
  proto_sources.extend([str(s) for s in Glob(os.path.join(proto_dir, string))])
source_target = [(s,  os.path.splitext(s)[0] + '.pb.h', os.path.splitext(s)[0] + '.pb.cc') for s in proto_sources]

for st in source_target:
  pb = environment.Command(
    [st[1], st[2]],
    st[0],
    'protoc --cpp_out=' + build_dir + ' ' + st[0]
  )

# Specify library
cc_files = [
  'listener/*.cc',
  'parser/*.cc',
  'renderer/*.cc',
  'renderer/sampler/*.cc',
  'renderer/shader/*.cc',
  'scene/*.cc',
  'scene/geometry/*.cc',
  'util/*.cc',
]
lib_sources = [Glob(cc_file) for cc_file in cc_files] + [st[2] for st in source_target]

p = environment.Library('raytracer', lib_sources)
environment.Append(LIBS='raytracer')
environment.Append(LIBS='glut')       # Only the binary needs to link against glut (not included for library).
environment.Append(LIBPATH='.')

# Specify binaries
environment.Program('raytracer.cc')

# This is how to force dependencies.
# environment.Depends(p, pb)
