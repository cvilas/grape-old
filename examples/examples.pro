TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += ConfigFile \
            Joystick \
            SerialPortMonitor \
            2DGraphics \
            Desktop \
            TcpEchoServer
unix:!android: SUBDIRS += Inventor
