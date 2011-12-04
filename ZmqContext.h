#ifndef ZMQCONTEXT_H
#define ZMQCONTEXT_H

#include <QString>

class ZmqContext
{
  friend class ZmqSocket;
public:
  ZmqContext(int iothreads, int defaultLinger);
  ~ZmqContext();
  static ZmqContext *instance(int iothreads=4, int defaultLinger = 0) {
    if (!self_)
        self_ = new ZmqContext(iothreads, defaultLinger);
    return self_;
  }

  static int majorVersion();
  static int minorVersion();
  static int patchVersion();
  static QString version();

  int linger() const { return linger_; }
  void setLinger(int msec) { linger_ = msec; }

private:
  static ZmqContext *self_;
  void *context_;
  int linger_;

};

#endif // ZMQCONTEXT_H
