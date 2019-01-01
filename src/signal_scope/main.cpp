#include <qapplication.h>
#include "mainwindow.h"
#include "rosthread.h"

#include "qjson.h"
#include <QMap>
#include <QVariant>

#include <iostream>


int main(int argc, char **argv)
{

  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}
