# grape

GRAPE stands for Graphical Real-time Application Prototyping Environment. This is forever work-in-progress as I tend to
use this repository to collect useful bits of code I have written over time  

## Introduction

Grape consists of two components, an embedded Target platform and a Supervisor. The Target is the computing unit (could
be a mobile processor board with sufficient IO, possibly running a hard-RTOS) that implements data acquisition, 
data processing algorithms and closed loop control. The Supervisor is a logically separate unit (could be another computer
board or another process) that provides visualisation, tuning, logging and debugging. Multiple supervisor guis can be 
attached to embedded processes on the Target to allow online parameter tuning, logging, on/off termination and control 
frequency. Key functionalities are efficient IO and good quality visualisation. Users extend functionality with algorithms.

C++11 has features that make it much easier to build real-time software, especially the _soft_ variety. Given careful
usage of the language features, it also benefits design of hard-real-time software that must run at several kHz. My
inspiration for this project is QMotor - an excellent QNX-based hard-realtime control system developed at Clemson 
University during my grad school days. What I am trying to do here is bring some of the features of QMotor, developed 
in the C++98 era, to todays standards. My design guides are (see docs folder):
- QMotor 3.0 and the QMotor Robotic Toolkit: A PC-Based Control Platform
- Design and Implementation of the Robotic Platform
- NSTX-U Advances in Real-time C++11 on Linux

## Design Philosophy

- A core set of software building blocks for real-time control (soft RT, as I do not work on demanding hard real time applications anymore)
- Unrestricted license (I would love it if this finds applications in schools and universities).
- Simplicity: Easy to instrument existing software with minimum coding and performance overhead
- Portability: Linux, Android
- Lightweight: can be installed on embedded platforms such as Arm-based boards
- Minimum external dependencies: Depend on few stable third party libraries such as Qt
- Message passing architecture as the backbone
  - Publish-Subscribe model
  - Inter-process and interprocessor (distributed across network)
- True distributed architecture
  - Bits run on multiple hardware targets
  - Allow RT task and GUI task separation - GUI runs on a separate process or even on a separate desktop 
- Extendable: Can be used as a basis to develop complex applications

## Technologies

- Language: C++14
- Messaging backbone: FastRTPS
- Graphics and 3D: Qt
- Build system: cmake 
- Revision control: git, github
- IDE - Qt Creator
- Unit testing - ctest
- Static analysis - clang-tidy, clang-format, Valgrind
- Documentation - doxygen, Dia
- Graphic design - Inkscape
- OS: Linux, Android
- Coding standard: https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md
- C++ coding style guide - http://wiki.ros.org/CppStyleGuide

## Core Functionality

- Distributed messaging
- Logging
- Configuration files
- Timed control loop
- Graphing
