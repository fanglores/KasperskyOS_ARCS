# KasperskyOS-ARCS 

System for controlling autonomous robot. Current implementation designed for Waveshase Alpha Bot. The point is in making control system, what sends moving commands via TCP\IP on Robot, Robot executes commands.  

### Repository structure
    .  
    ├── KOS_TCP-IP/                       # general folder for the project  
    |    ├── client/                      # net2 client code folder  
    |    ├── server/                      # net2 server code folder
    |    ├── einit/                       # einit entity  
    |    ├── env/                         # env entity  
    |    ├── vfs/                         # netvfs entity  
    |    ├── resources/                   # edl files  
    |    | 
    |    ├── CMakeLists.txt               # top-level CMake list
    |    └── cross-build.sh               # script for compiling and executing code under qemu
    |  
    |  
    ├── KOS_GPIO/
    |    ├── client/                      # gpio client code folder  
    |    ├── einit/                       # einit entity  
    |    ├── resources/                   # edl files  
    |    | 
    |    ├── CMakeLists.txt               # top-level CMake list
    |    ├── cross-build.sh               # script for compiling and executing code under qemu
    |    ├── image-build.sh               # script for creating kos-image for RPi 4B  
    |    └── mount-image.sh               # script for mounting kos-image on sd card  
    |  
    ├── .gitignore  
    └── README.md                         # you are here
  
### To-do list
  - [x] Add to-do list
  - [ ] Write further tasks in here
  
fanglores, Moscow Aviation Institute  
Moscow, 2022
