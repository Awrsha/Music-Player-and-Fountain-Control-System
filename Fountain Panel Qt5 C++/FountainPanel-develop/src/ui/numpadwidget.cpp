#include "numpadwidget.h"
#include "ui_numpadwidget.h"
#include <QLineEdit>

NumpadWidget::NumpadWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::NumpadWidget)
{
  ui->setupUi(this);
}

NumpadWidget::~NumpadWidget()
{
  delete ui;
}

void NumpadWidget::showEvent(QShowEvent *)
{

}
#include <QDebug>

void NumpadWidget::addNumberToFocusedWidget(const int &number)
{
  QWidget* widget;
  if (focusWidget() != NULL)
    {
      widget = focusWidget();
    }
  else
    {
      widget = setWidget;
    }
  if (widget == NULL)
    return;

  if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget))
    {
      lineEdit->setText(lineEdit->text() + QString::number(number));
    }
}

void NumpadWidget::setSetWidget(QWidget *value)
{
  setWidget = value;
}


void NumpadWidget::on_removeButton_clicked()
{
  emit removeButtonClicked();
}

void NumpadWidget::on_num1Button_clicked()
{
  addNumberToFocusedWidget(1);
}

void NumpadWidget::on_num2Button_clicked()
{
  addNumberToFocusedWidget(2);
}

void NumpadWidget::on_num3Button_clicked()
{
  addNumberToFocusedWidget(3);
}

void NumpadWidget::on_num4Button_clicked()
{
  addNumberToFocusedWidget(4);
}

void NumpadWidget::on_num5Button_clicked()
{
  addNumberToFocusedWidget(5);
}

void NumpadWidget::on_num6Button_clicked()
{
  addNumberToFocusedWidget(6);
}

void NumpadWidget::on_num7Button_clicked()
{
  addNumberToFocusedWidget(7);
}

void NumpadWidget::on_num8Button_clicked()
{
  addNumberToFocusedWidget(8);
}

void NumpadWidget::on_num9Button_clicked()
{
  addNumberToFocusedWidget(9);
}

void NumpadWidget::on_num0Button_clicked()
{
  addNumberToFocusedWidget(0);
}

void NumpadWidget::on_okButton_clicked()
{
  emit okButtonClicked();
}
