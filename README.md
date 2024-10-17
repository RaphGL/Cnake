# Cnake
This project constains 2 versions of the game.
A legacy version in C11 and a new version in C++20. 
None is better than the other, they're simply different flavors of the same game (the C++ one is partially a rewrite).

![Screenshot from 2023-07-16 19-29-32](https://github.com/RaphGL/Cnake/assets/28673457/80bd075e-de33-4295-a3b1-85b0acc48cfc)
![Screenshot from 2023-07-16 19-31-34](https://github.com/RaphGL/Cnake/assets/28673457/72ff51a2-4be2-4632-966a-bdcec8c4a575)
![Screenshot from 2023-07-16 19-31-40](https://github.com/RaphGL/Cnake/assets/28673457/9dbb33ab-66bb-4e54-8ac0-65a3904341ad)

# How to compile
Build dependencies:
- CMake (build system)
- Ncurses (library)
- A compiler with C++20 support

```sh
$ cmake . -DCMAKE_BUILD_TYPE=Release
$ make
```

After compiling you'll have a `cnake` for the C11 version and a `cnakepp` for the C++20 version.

# Controls
Move with WASD or the arrows keys  
Press `p` to pause  
Stay pressed to go faster  

The game can be quit with `ctrl + c` or through the pause menu
