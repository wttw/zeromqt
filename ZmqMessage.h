#ifndef ZMQMESSAGE_H
#define ZMQMESSAGE_H

#include <zmq.h>

#ifndef ZMQ_VERSION
#error ZMQ_VERSION not defined - zmq.h is too old - version 2.1.0 is required
#endif

#if ZMQ_VERSION < 20100
#error zmq.h is too old - version 2.1.0 is required
#endif

#include <QString>
#include <QByteArray>
#include <string.h>
#include <assert.h>

#define USE_EXCEPTIONS 1

#ifdef USE_EXCEPTIONS
#include "ZmqException.h"
#define THROW() throw ZmqException()
#else
#define THROW() do { error_ = ZMQERRNO; \
errorString_ = QString::fromAscii(zmq_strerror(error_)); return; }
#endif

class ZmqMessage : private zmq_msg_t
{
  friend class ZmqSocket;
public:
  ZmqMessage() {
    if(0 != zmq_msg_init(this)) THROW();
  }

  ZmqMessage(size_t size_){
    if(0 != zmq_msg_init_size(this, size_)) THROW();
  }

  ZmqMessage(void *data_, size_t size_, zmq_free_fn *ffn=0, void *hint=0) {
    if(0 != zmq_msg_init_data(this, data_, size_, ffn, hint)) THROW();
  }

  ZmqMessage(const QByteArray& b) {
    if(0 != zmq_msg_init_size(this, b.size())) THROW();
      memcpy(data(), b.constData(), b.size());
  }

  ~ZmqMessage() {
    int rc = zmq_msg_close(this);
    assert(rc == 0);
  }
  void copy(ZmqMessage *msg) {
    if(0 != zmq_msg_copy(this, (zmq_msg_t*)msg)) THROW();
  }
  void move(ZmqMessage *msg) {
    if(0 != zmq_msg_move(this, (zmq_msg_t*)msg)) THROW();
  }
  void clone(ZmqMessage *msg) {
    rebuild(msg->size());
    memcpy(data(), msg->data(), size());
  }

  void clear() { rebuild(); }

  void rebuild() {
    if(0 != zmq_msg_close(this)) THROW();
    if(0 != zmq_msg_init(this)) THROW();
  }
  void rebuild(size_t size_) {
    if(0 != zmq_msg_close(this)) THROW();
    if(0 != zmq_msg_init_size(this, size_)) THROW();
  }
  void rebuild(void *data_, size_t size_, zmq_free_fn *ffn=0, void *hint=0) {
    if(0 != zmq_msg_close(this)) THROW();
    if(0 != zmq_msg_init_data(this, data_, size_, ffn, hint)) THROW();
  }

  void rebuild(const QByteArray& b) {
    if(0 != zmq_msg_close(this)) THROW();
    if(0 != zmq_msg_init_size(this, b.size())) THROW();

    memcpy(data(), b.constData(), b.size());
  }

  size_t size() { return zmq_msg_size(this); }
  void *data() { return zmq_msg_data(this); }

  QByteArray toByteArray() {
    return QByteArray((const char *)data(), size());
  }

private:
  int error_;
  QString errorString_;
};

#endif // ZMQMESSAGE_H
