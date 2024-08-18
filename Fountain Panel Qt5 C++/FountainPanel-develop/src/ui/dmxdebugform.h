#ifndef DMXDEBUGFORM_H
#define DMXDEBUGFORM_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QIntValidator>

#include <QSpinBox>
#include <QLabel>

#include <QMap>
#include <QVector>

#include "src/core/dmxinterface.h"

namespace Ui {
  class DmxDebugForm;
}

class DmxDebugForm : public QWidget
{
  Q_OBJECT

public:
  explicit DmxDebugForm(QWidget *parent = 0);
  ~DmxDebugForm();

private slots:
  void hideZeroSpinBoxes();


  void on_clearAllPushButton_clicked();

  void on_setPushButton_clicked();

  void on_resetPushButton_clicked();

  void on_sendPushButton_clicked();

private:
  QVector<quint8> getDmxFrameData();
  QVector<quint8> getZeroFrameData();

  Ui::DmxDebugForm *ui;
  QMap<quint16, QSpinBox*> spinBoxMap;
  QMap<quint16, QLabel*> labelMap;

//  QIntValidator intValidator;
};

#endif // DMXDEBUGFORM_H
