cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cnake
  DESCRIPTION "snake game written in C and C++"
  HOMEPAGE_URL "https://github.com/RaphGL/Cnake"
  LANGUAGES C;CXX
)

# --- Original C implementation ---

add_executable(cnake
  cnake_src/colors.c  
  cnake_src/colors.h  
  cnake_src/game.c  
  cnake_src/game.h  
  cnake_src/leaderboard.c  
  cnake_src/leaderboard.h  
  cnake_src/main.c  
  cnake_src/optionmenu.c  
  cnake_src/optionmenu.h  
  cnake_src/startscreen.c  
  cnake_src/startscreen.h  
  cnake_src/vector.c  
  cnake_src/vector.h
)

target_compile_features(cnake PUBLIC c_std_11)

target_link_libraries(cnake ncursesw m)

target_compile_options(cnake PUBLIC -Wall -Wextra -pedantic-errors)

# --- Improved C++ implementation ---

add_executable(cnakepp
  cnakepp_src/consts.hpp  
  cnakepp_src/food.cpp  
  cnakepp_src/food.hpp  
  cnakepp_src/game_manager.cpp  
  cnakepp_src/game_manager.hpp  
  cnakepp_src/main.cpp  
  cnakepp_src/snake.cpp  
  cnakepp_src/snake.hpp
)

target_link_libraries(cnakepp ncursesw)

target_compile_features(cnakepp PRIVATE cxx_std_20)

target_compile_options(cnakepp PUBLIC -Wall -Wextra -pedantic-errors)

