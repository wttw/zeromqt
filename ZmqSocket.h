#ifndef ZMQSOCKET_H
#define ZMQSOCKET_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QSocketNotifier>

#include "ZmqMessage.h"

class ZmqSocket : public QObject
{
  Q_OBJECT
public:
      explicit ZmqSocket(int type, QObject *parent = 0);
  ~ZmqSocket();

  bool getOpt(int opt_name, void *opt_value, size_t *opt_length);

  bool setOpt(int opt_name, const void *opt_value, size_t opt_length);
  bool setOpt(int opt_name, const QByteArray& b) {
    return setOpt(opt_name, b.constData(), b.size());
  }
  bool setOpt(int opt_name, int value) {
    size_t size = sizeof(value);
    return setOpt(opt_name, &value, size);
  }

  void setIdentity(const QByteArray& name);
  QByteArray identity();

  void setLinger(int msec);
  int linger();

  void subscribe(const QByteArray& filter);
  void unsubscribe(const QByteArray& filter);

  void bind(const char *addr_);
  void connectTo(const char *addr_);

  bool send(ZmqMessage& msg, int flags=ZMQ_NOBLOCK) {
    int rc = zmq_send(socket_, &msg, flags);
    if(0 == rc) return true;
    if(-1 == rc && zmq_errno() == EAGAIN) return false;
    THROW();
  }

  bool send(const QByteArray& b) {
    ZmqMessage msg(b);
    return send(msg);
  }

  bool recv(ZmqMessage *msg_, int flags=ZMQ_NOBLOCK) {
    int rc = zmq_recv(socket_, msg_, flags);
    if(0 == rc) return true;
    if(-1 == rc && zmq_errno() == EAGAIN) return false;
    THROW();
  }

  QList<QByteArray> recv();

  int error() const { return error_; }
  QString errorString() const { return errorString_; }

signals:
  void readyRead();
  void readyWrite();
public slots:

protected slots:
  void activity();
private:
  void *socket_;
  QSocketNotifier *notifier_;
  int error_;
  QString errorString_;
};

#endif // ZMQSOCKET_H
