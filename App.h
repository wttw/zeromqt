#ifndef APP_H
#define APP_H

#include <QObject>

class ZmqSocket;

class App : public QObject
{
  Q_OBJECT
public:
    App();
  protected slots:
    void start();
    void squawk();
    void heard();
    void ndy();
private:
    ZmqSocket *sock;
    QByteArray key;
};

#endif // APP_H
