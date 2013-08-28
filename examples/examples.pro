TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += ConfigFile \
            Joystick \
            SerialPortMonitor \
            2DGraphics \
            Desktop \
            TcpEchoServer
android: SUBDIRS +=
else: SUBDIRS += Inventor
