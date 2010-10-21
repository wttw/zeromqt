#include "App.h"

#include <QTimer>
#include <QStringList>
#include <QCoreApplication>
#include <QtDebug>
#include <QDateTime>

#include "ZmqSocket.h"
#include "ZmqMessage.h"

App::App()
{
  QTimer::singleShot(0, this, SLOT(start()));
}

void App::start()
{
  QStringList args = QCoreApplication::arguments();
  if(args.size() < 3) {
    qWarning() << "Usage: " << args.front() << " client|server <key>";
    QCoreApplication::exit();
    return;
  }

  key = args.at(2).toLocal8Bit();

  QTimer *ndyt = new QTimer(this);
  connect(ndyt, SIGNAL(timeout()), this, SLOT(ndy()));
  ndyt->setInterval(5000);
  ndyt->start();

  if(args.at(1) == "client") {
    sock = new ZmqSocket(ZMQ_SUB, this);
    connect(sock, SIGNAL(readyRead()), this, SLOT(heard()));
    sock->subscribe(key);
    sock->connectTo("tcp://127.0.0.1:3782");
  }

  if(args.at(1) == "server") {
   sock = new ZmqSocket(ZMQ_PUB, this);
    sock->bind("tcp://127.0.0.1:3782");
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(squawk()));
    t->setInterval(1000);
    t->start();
  }
}

void App::squawk()
{
  QString s =  QDateTime::currentDateTime().toString();
  qDebug() << "Squawking " << s;
  sock->send(key + s.toLocal8Bit());
}

void App::heard()
{
  QList<QByteArray> r = sock->recv();
  for(QList<QByteArray>::const_iterator i=r.constBegin(); i!=r.constEnd(); ++i) {
    qDebug() << "Received " << *i;
  }
}

void App::ndy()
{
  qDebug() << "Not dead yet";
}
