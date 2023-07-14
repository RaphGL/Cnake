project(
  'cnake', 'c', 
  default_options: [
    'c_std=c11', 
    'warning_level=1', 
    'buildtype=release'] 
)

executable(
  'cnake', 
  ['src/main.c'],
  dependencies: dependency('ncursesw')
)
