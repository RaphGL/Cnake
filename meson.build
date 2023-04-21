project(
  'cnakepp', 'cpp', 
  default_options: [
    'cpp_std=c++17', 
    'warning_level=1', 
    'buildtype=release'] 
)

executable(
  'cnakepp', 
  ['src/bar.cpp', 'src/food.cpp', 'src/main.cpp', 'src/snake.cpp'],
  dependencies: dependency('ncursesw')
)
