#include "ZmqContext.h"

#include <zmq.h>
#include <stdio.h>

#include <QtGlobal>

ZmqContext *ZmqContext::self_ = 0;

ZmqContext::ZmqContext(int iothreads, int defaultLinger) : linger_(defaultLinger)
{
  Q_ASSERT(!self_);
  context_ = zmq_init(iothreads);
  if(!context_) {
    if(errno == EINVAL) {
      qFatal("Invalid number of iothreads (%d) in ZmqContext", iothreads);
    } else {
      qFatal("Error in ZMQContext: %d %s", errno, zmq_strerror(errno));
    }
  }
}

ZmqContext::~ZmqContext()
{
  zmq_term(context_);
}

int ZmqContext::majorVersion()
{
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  return major;
}

int ZmqContext::minorVersion()
{
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  return minor;
}

int ZmqContext::patchVersion()
{
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  return patch;
}

QString ZmqContext::version()
{
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  return QString("%1.%2.%3").arg(major).arg(minor).arg(patch);
}
