# The MIT License (MIT)
#
# Copyright (c) 2015 dinowernli
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os

build_dir = 'build'
proto_dir = 'proto'

# A set of paths from which to ignore warnings
ignore_warnings = ['/home/dino/include']

environment = Environment(
  CCFLAGS = ['-Wall', '-pipe', '-std=c++0x', '-pthread'] + ['-isystem' + path for path in ignore_warnings],
  LIBS = ['-lpthread'],
  ENV = os.environ,
  CPPPATH = ['.'],
)

def IsActive(arg):
  return ARGUMENTS.get(arg) == '1'

if IsActive('debug'):
  environment.Append(CCFLAGS = ['-g'])
else:
  # Build optimizes binaries by default.
  environment.Append(CCFLAGS = ['-O3'])
  environment.Append(CCFLAGS = ['-DNDEBUG'])
  environment.Append(CCFLAGS = ['-march=native'])

if IsActive('profile'):
  environment.Append(CCFLAGS = ['-pg'])
  environment.Append(LINKFLAGS = ['-pg'])

#environment.Replace(CXX = "clang")
#environment.Append(CCFLAGS = ['-stdlib=libc++'])

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
  'scene/light/*.cc',
  'scene/texture/*.cc',
  'util/*.cc',
]
lib_sources = [Glob(cc_file) for cc_file in cc_files] + [st[2] for st in source_target]

lib_target = environment.Library('raytracer', lib_sources)
environment.Prepend(LIBS='raytracer')
environment.ParseConfig('pkg-config --cflags --libs glu')
environment.Append(LIBS='glut')  # Does not seem to have a pkg-config file
environment.Append(LIBPATH='.')

# Specify binaries
raytracer = environment.Program('raytracer.cc')
Default(raytracer)

# This is how to force dependencies.
# environment.Depends(lib_target, pb)


### Build unit tests.

test_environment = environment.Clone()
test_environment.Append(LIBS='-lgtest')
test_environment.Prepend(LIBS='-lgtest_main')
test_cc_files = [
  'test/renderer/*.cc',
  'test/scene/geometry/*.cc',
  'test/scene/light/*.cc',
  'test/scene/texture/*.cc',
  'test/util/*.cc',
]
test_sources = [Glob(cc_file) for cc_file in test_cc_files]
test_environment.Program('unit_tests', test_sources)
test_environment.Alias('test', ['unit_tests'])
