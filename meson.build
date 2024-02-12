project(
    'beg',
    ['c', 'cpp'], 
    version: '0.1.0',
    default_options: ['warning_level=2', 'cpp_std=c++20']
)

inc = [
    include_directories('include', 'include/ext')
]
src = [
    'src/ext/glad.c',

    'src/bmath.cpp',
    'src/affine.cpp',
    'src/shader.cpp',
    'src/camera.cpp',
    'src/color.cpp',
    'src/model.cpp',
    'src/ecs.cpp',
    'src/input.cpp',
    'src/game.cpp',
    'src/basic.cpp',
    'src/renderable.cpp',
    'src/beg.cpp'
]

args = ['-std=c++20', '-Wall', '-Weffc++', '-Wextra', '-Wconversion', '-Wsign-conversion']

deps = [
    dependency('glm'),
    dependency('glfw3'),
    dependency('GL'),
    dependency('dl')
]

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'cpp')
endif

target = executable(
    'beg',
    src,
    include_directories: inc,
    dependencies: deps,
    cpp_args: args
)

test('basic', target)