#ifndef _PYTHONMESSAGEINSPECTOR_H_
#define _PYTHONMESSAGEINSPECTOR_H_

#include "rossubscriber.h"
#include "rosthread.h"

#include <PythonQt.h>

#include <QVariant>
#include <QMap>
#include <QString>

class PythonMessageInspector : public ROSSubscriber
{
  Q_OBJECT

public:

  PythonMessageInspector(PythonQtObjectPtr decodeCallback, QObject* parent=0) : ROSSubscriber(parent)
  {
    mDecodeCallback = decodeCallback;
  }

  void subscribe(ros::NodeHandle* rosHandle)
  {
  }

  void handleMessageOnChannel()
  {
  }


  QMap<QString, PythonQtObjectPtr> mChannelMap;
  PythonQtObjectPtr mDecodeCallback;
};

#endif
