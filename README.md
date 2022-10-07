# KasperskyOS-ARCS 

System for controlling an autonomous robot. Current implementation designed for Waveshase Alpha Bot. The point is in making a control system, what sends moving commands via TCP\IP on Robot, Robot executes commands. Server control code should be implemented on Python using OpenCV libraries. Processing images from the cameras on the walls, it will create controlling commands for the robot.   

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
    |    ├── server/                      # cutted server source code folder just for connecting to the KasperskyOS client and debugging
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
  - [ ] Split client source code into .h and .cpp files, using OOP: creating classes and its` methods
  - [ ] Reverse robot movement (it should ride driving wheels ahead; currently - back)
  - [ ] Establish the control of the robot via TCP by server
  - [ ] And much more...
  
Konstantin Tsaturyan, Moscow Aviation Institute  
fanglores  
Moscow, 2022
