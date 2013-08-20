grape
=====

GRAPE stands for Graphical Real-time Application Prototyping Environment (Sounds ambitious, I know). I essentially 
started this project to collect various bits of useful code I have written over many years into a single repository. 

Goals
-----
- A core set of software building blocks for robotics (threading, logging, messaging, algebra)
- Portable (Windows, Linux, Android, ..)
- Lightweight (can be installed on embedded platforms such as Arm)
- Simple - able to develop a realtime control loop very easily - QMotor model
- Leverages existing high-quality and stable libraries such as Qt, Coin3D, OpenCV, PCL, etc.
- Open source and unrestrictive licensing

Core Libraries
--------------
- Config files
- Messaging (IPC)
- Timing
- Logging
- Realtime Control loop
- License handling
- Input device handling (joysticks, etc)
- Sensors (GPS, INS)
- Communication (sockets, serial port)
- Robotics: trajectory generators, path planning, maths
- Filters (Sensor fusion - INS/GPS, kalman filter)
- Scripting
- Multimedia (audio and video recording, playback)
- Graphics (plotting, 3D visualisation)

A lot of the stuff above is not currently in the library - as this is something that I work on in my free time, 
this library is probably going to be eternally in work-in-progress state. 
