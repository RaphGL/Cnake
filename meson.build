project(
  'cnake', 'c', 
  default_options: [
    'c_std=c11', 
    'warning_level=1', 
    'buildtype=release'] 
)

cc = meson.get_compiler('c')

executable(
  'cnake', 
  ['src/main.c', 'src/colors.c', 'src/startscreen.c', 'src/vector.c'],
  dependencies: [dependency('ncursesw'), cc.find_library('m')]
)
