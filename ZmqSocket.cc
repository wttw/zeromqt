#include <zmq.h>
#include <errno.h>
#include <stdint.h>

#include "ZmqSocket.h"
#include "ZmqContext.h"

#include <QtDebug>

#if 1
#define ZMQERRNO errno
#else
#define ZMQERRNO zmq_errno()
#endif

#define USE_EXCEPTIONS 1

#ifdef USE_EXCEPTIONS
#include "ZmqException.h"
#define THROW() throw ZmqException()
#else
#define THROW() do { error_ = ZMQERRNO; \
errorString_ = QString::fromAscii(zmq_strerror(error_)); }
#endif

ZmqSocket::ZmqSocket(int type, QObject *parent) :
    QObject(parent)
{
  ZmqContext *ctx = ZmqContext::instance();
  socket_ = zmq_socket(ctx->context_, type);
  setLinger(ctx->linger());
  // FIXME - on Windows getsockopt returns a SOCKET opaque handle
  int fd;
  size_t size=sizeof(fd);
  getOpt(ZMQ_FD, &fd, &size);
  notifier_ = new QSocketNotifier(fd, QSocketNotifier::Read, this);
  connect(notifier_, SIGNAL(activated(int)), this, SLOT(activity()));
}

ZmqSocket::~ZmqSocket()
{
  zmq_close(socket_);
}

bool ZmqSocket::getOpt(int opt_name, void *opt_value, size_t *opt_length) {
  if(zmq_getsockopt(socket_, opt_name, opt_value, opt_length)) {
    THROW();
    return false;
  }
  return true;
}

bool ZmqSocket::setOpt(int opt_name, const void *opt_value, size_t opt_length) {
  if(zmq_setsockopt(socket_, opt_name, opt_value, opt_length)) {
    THROW();
    return false;
  }
  return true;
}


void ZmqSocket::setIdentity(const QByteArray &name)
{
  setOpt(ZMQ_IDENTITY, const_cast<char*>(name.constData()), name.size());
}

QByteArray ZmqSocket::identity()
{
  char buff[256];
  size_t size = sizeof(buff);
  if(getOpt(ZMQ_IDENTITY, buff, &size)) {
    return QByteArray(buff, size);
  }
  return QByteArray();
}

void ZmqSocket::setLinger(int msec)
{
  setOpt(ZMQ_LINGER, msec);
}

int ZmqSocket::linger()
{
  int msec=-1;
  size_t size = sizeof(msec);
  getOpt(ZMQ_LINGER, &msec, &size);
  return msec;
}

void ZmqSocket::subscribe(const QByteArray &filter)
{
  setOpt(ZMQ_SUBSCRIBE, filter);
}

void ZmqSocket::unsubscribe(const QByteArray &filter)
{
  setOpt(ZMQ_UNSUBSCRIBE, filter);
}

void ZmqSocket::activity()
{
  uint32_t flags;
  size_t size = sizeof(flags);
  if(!getOpt(ZMQ_EVENTS, &flags, &size)) {
    qWarning("Error reading ZMQ_EVENTS in ZMQSocket::activity");
    return;
  }
  if(flags & ZMQ_POLLIN) {
    emit readyRead();
  }
  if(flags & ZMQ_POLLOUT) {
    emit readyWrite();
  }
  if(flags & ZMQ_POLLERR) {
    // ?
  }
}

void ZmqSocket::bind(const char *addr_)
{
  if(0 != zmq_bind(socket_, addr_)) THROW();
}

void ZmqSocket::connectTo(const char *addr_)
{
  if(0 != zmq_connect(socket_, addr_)) THROW();
}

QList<QByteArray> ZmqSocket::recv() {
  QList<QByteArray> ret;
  ZmqMessage m;
  while(recv(&m)) {
    ret.append(m.toByteArray());
    m.clear();
  }
  return ret;
}
