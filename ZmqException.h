#ifndef ZMQEXCEPTION_H
#define ZMQEXCEPTION_H
#include <zmq.h>
#include <stdexcept>

class ZmqException : public std::exception
{
public:
  ZmqException() : errno_ (zmq_errno()) {}
  virtual const char *what () const throw () {
     return zmq_strerror (errno_);
  }
  int error() const { return errno_; }
private:
    int errno_;
};

#endif // ZMQEXCEPTION_H
