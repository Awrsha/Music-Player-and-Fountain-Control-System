#include "iodebugform.h"
#include "ui_iodebugform.h"
#include <QDebug>
#include "src/util/projecttheme.h"

IoDebugForm::IoDebugForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::IoDebugForm)
{
  ui->setupUi(this);

  addInputGui();
  addOutputGui();

}

IoDebugForm::~IoDebugForm()
{
  delete ui;
}

void IoDebugForm::addInputGui()
{
  QGridLayout* gridLayout = new QGridLayout();
  ui->inputScrollAreaWidgetContents->setLayout(gridLayout);
  gridLayout->setSpacing(4);
  int width = 4;
  for (int i = 0 ; i < 40; ++i)
    {
      QCheckBox* checkBox = new QCheckBox(ui->inputScrollAreaWidgetContents);
      checkBox->setFixedSize(120, 30);
      checkBox->setDisabled(false);
      checkBox->setText("Input " + QString((i<10)?" ":"") + QString::number(i));
      checkBox->setLayoutDirection(Qt::RightToLeft);
      gridLayout->addWidget(checkBox, (i) / width, (i) % width);

      inputMap.insert(i, checkBox);

    }



}

void IoDebugForm::addOutputGui()
{
  QGridLayout* gridLayout = new QGridLayout();
  ui->outputScrollAreaWidgetContents->setLayout(gridLayout);
  gridLayout->setSpacing(4);
  int width = 4;
  for (int i = 0 ; i < 40; ++i)
    {
      QCheckBox* checkBox = new QCheckBox(ui->outputScrollAreaWidgetContents);
      checkBox->setFixedSize(120, 30);
      checkBox->setChecked(false);
      checkBox->setText("Output " + QString((i<10)?" ":"") + QString::number(i));
      checkBox->setLayoutDirection(Qt::RightToLeft);
      gridLayout->addWidget(checkBox, (i) / width, (i) % width);
      outputMap.insert(i, checkBox);
      outputInverseMap.insert(checkBox, i);

      connect(checkBox, SIGNAL(toggled(bool)),
              SLOT(onCheckboxToggled()));
    }
}

void IoDebugForm::onReceiveInputUpdate(const DigialInputResponse &response)
{
  foreach (quint8 key, inputMap.keys()) {
      inputMap.value(key)->setChecked(response.getResponse(indexToInputAddress(key)));
    }
}

void IoDebugForm::onReceiveOutputUpdate(const DigialOutputResponse &response)
{
  foreach (quint8 key, inputMap.keys()) {
      outputMap.value(key)->setChecked(response.getResponse(indexToOutputAddress(key)));
    }
}

void IoDebugForm::onCheckboxToggled()
{
  if (QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender()))
    {
      emit sendIOCommand(indexToOutputAddress(outputInverseMap.value(checkbox)), checkbox->isChecked())  ;
    }
}
