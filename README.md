# KasperskyOS-ARCS 

System for controlling autonomous robot. Current implementation designed for Waveshase Alpha Bot. The point is in making control system, what sends moving commands via TCP\IP on Robot, Robot executes commands.  

### Repository structure
    .  
    ├── KOS_Client/                       # the latest version of the project
    |    ├── server/                      # GPIO&TCP/IP client source code folder  
    |    ├── einit/                       # einit entity  
    |    ├── env/                         # env entity  
    |    ├── vfs/                         # vfs entity  
    |    ├── resources/                   # edl files  
    |    | 
    |    ├── CMakeLists.txt               # top-level CMake list
    |    ├── cross-build.sh               # script for compiling and executing code under qemu
    |    ├── image-build.sh               # script for creating kos-image for RPi 4B  
    |    └── mount-image.sh               # script for mounting kos-image on sd card  
    |  
    |  
    ├── server/                           # linux server code  
    |    ├── server/                      # server source code folder for connecting to the KasperskyOS client
    |    | 
    |    ├── CMakeLists.txt               # top-level CMake list
    |    └── cross-build.sh               # script for compiling the code and creating executable
    |  
    |  
    ├── .gitignore  
    └── README.md  
  
### To-do list
  - [x] Add to-do list
  - [ ] Unite GPIO and NetVFS already
  
Konstantin Tsaturyan (aka fanglores), Moscow Aviation Institute  
Moscow, 2022
