#include "dmxdebugform.h"
#include "ui_dmxdebugform.h"
#include <QSpacerItem>
#include <QDebug>

DmxDebugForm::DmxDebugForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DmxDebugForm)
{
  ui->setupUi(this);

//  QGridLayout* gridLayout = new QGridLayout();
  QVBoxLayout* verticalLayout = new QVBoxLayout;

//  ui->scrollAreaWidgetContents->setLayout(gridLayout);
  ui->scrollAreaWidgetContents->setLayout(verticalLayout);
  //  intValidator.setTop(255);
  //  intValidator.setBottom(0);

//  gridLayout->setSpacing(4);


  int width = 4;
  for (int i = 1 ; i < 513; ++i)
    {
      QSpinBox* spinBox = new QSpinBox(ui->scrollAreaWidgetContents);
      QLabel *label = new QLabel();
      //      spinBox->setValidator(&intValidator);
      spinBox->setFixedSize(50, 30);
      label->setFixedSize(80, 30);
      label->setText("No " + QString::number(i));
      spinBox->setMinimum(0);
      spinBox->setMaximum(255);

      QHBoxLayout* boxLayout = new QHBoxLayout;
      boxLayout->addSpacing(200);
      boxLayout->addWidget(label);
      boxLayout->addWidget(spinBox);
      boxLayout->addSpacing(200);
      boxLayout->setSpacing(20);
//      gridLayout->addLayout(boxLayout, (i-1) / width, (i-1) % width);
      verticalLayout->addLayout(boxLayout);
//      gridLayout->addWidget(spinBox, (i) / width, (i) % width);

      spinBoxMap.insert(i, spinBox);
      labelMap.insert(i, label);
    }
//  QSpacerItem* spacerItem = new QSpacerItem(10, 40, QSizePolicy::Expanding, QSizePolicy::Fixed);
//  verticalLayout->addSpacerItem(spacerItem);
  verticalLayout->addStretch();

  hideZeroSpinBoxes();
}

DmxDebugForm::~DmxDebugForm()
{
  delete ui;
}

void DmxDebugForm::hideZeroSpinBoxes()
{
  foreach (quint16 key, spinBoxMap.keys()) {
      if (spinBoxMap.value(key)->value() == 0)
        {
          spinBoxMap.value(key)->hide();
          labelMap.value(key)->hide();
        }
      else
        {
          spinBoxMap.value(key)->show();
          labelMap.value(key)->show();
        }
    }
}

void DmxDebugForm::on_clearAllPushButton_clicked()
{
  foreach (quint16 key, spinBoxMap.keys())
    {
      spinBoxMap.value(key)->setValue(0);
    }
  hideZeroSpinBoxes();
}

void DmxDebugForm::on_setPushButton_clicked()
{
  int count = ui->countSpinBox->value();
  int address = ui->startAddressSpinBox->value();
  int step = ui->stepSpinBox->value();
  int lastDmxChannelSize = 0;
  int value = ui->valueSpinBox->value();
  for (int i = 0 ; i < count; ++i)
    {

      if (spinBoxMap.contains(address))
        {
          spinBoxMap.value(address)->setValue(value);
        }
      address = address + step + lastDmxChannelSize;
    }
  hideZeroSpinBoxes();
}

void DmxDebugForm::on_resetPushButton_clicked()
{
  QVector<quint8> dmxFrameData = getZeroFrameData();

  DmxInterface::sendDmxData(dmxFrameData);
}

void DmxDebugForm::on_sendPushButton_clicked()
{
  QVector<quint8> dmxFrameData = getDmxFrameData();

  DmxInterface::sendDmxData(dmxFrameData);
}

QVector<quint8> DmxDebugForm::getDmxFrameData()
{
  QVector<quint8> dmxFrameData;
  for (int index = 0 ; index < DmxInterface::DMX_FRAME_COUNT ; ++index)
    {
      dmxFrameData.push_back(spinBoxMap.value(index + 1)->value());
    }
  return dmxFrameData;
}

QVector<quint8> DmxDebugForm::getZeroFrameData()
{
  QVector<quint8> dmxFrameData;
  for (int index = 0 ; index < DmxInterface::DMX_FRAME_COUNT; ++index)
    {
      dmxFrameData.push_back(0);
    }
  return dmxFrameData;
}
