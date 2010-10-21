Zeromqt - a [Qt][] binding for [ZeroMQ][]
=========================================

Zeromqt integrates ZeroMQ (version 2.1.0 and later) into the Qt event loop,
mapping ZeroMQ message events onto Qt signals.

It also provides an API that is more "Qt-like" than the native ZeroMQ C or C++
APIs, allowing messages to be handled as QByteArrays and providing classes
that are similar in style to QAbstractSocket.

Status
------

Pre-alpha.

There are no known bugs, but nor has there been any testing beyond
the included sample app. It's being used as part of a large commercial
application, though, so it'll be getting some testing and attention.

Usage
-----

To use Zeromqt in a Qt project, add this to the projects .pro file

    HEADERS += ZmqException.h ZmqMessage.h ZmqSocket.h ZmqContext.h
    SOURCES += ZmqSocket.cc ZmqMessage.cc
    LIBS += -lzmq

and copy the six Zmq* files into the project.

Exceptions
----------

Zeromqt uses C++ exceptions by default. You'll need to catch ZmqException
objects in the normal way (which on Qt probably involves inheriting from
QCoreAppliction and reimplementing notify()).

There is untested support in the code for replacing exceptions with Qt-style
errorString() error handling instead - look for USE_EXCEPTIONS

Sample
------

There's a single pub-sub sample included. Build it with qmake; make. Run the
server with "./pubsubsample server foo" and the client with
"./pubsubsample client foo".

More Information
----------------

[Word to the Wise Labs](http://labs.wordtothewise.com/zeromqt/)

[qt]:     http://qt.nokia.com/ "Qt"
[zeromq]: http://zeromq.com/   "ZeroMQ"
