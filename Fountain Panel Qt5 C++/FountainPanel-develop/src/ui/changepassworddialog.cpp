#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"
#include "src/core/data.h"

#include <QMessageBox>

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ChangePasswordDialog)
{
  ui->setupUi(this);
  setWindowTitle("Change Password");
  ui->curentPasswordLineEdit->setValidator(&intValidator);
  ui->newPasswordLineEdit->setValidator(&intValidator);
  ui->retypePasswordLineEdit->setValidator(&intValidator);

  connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
          SLOT(setFocusWidget()));
  connect(ui->numpadWidget, SIGNAL(removeButtonClicked()),
          SLOT(onRemoveButtonClicked()));
  connect(ui->numpadWidget, SIGNAL(okButtonClicked()),
          SLOT(on_changePasswordButton_clicked()));
}

ChangePasswordDialog::~ChangePasswordDialog()
{
  delete ui;
}
#include <QDebug>
void ChangePasswordDialog::on_changePasswordButton_clicked()
{
  if (ui->curentPasswordLineEdit->text().isEmpty() ||
      ui->newPasswordLineEdit->text().isEmpty() ||
      ui->retypePasswordLineEdit->text().isEmpty())
    {
      QMessageBox message(this);
      message.setText("Please fill all the inputs.");
      message.setDefaultButton(QMessageBox::Ok);
      message.exec();
      return;
    }

  int currentPass = ui->curentPasswordLineEdit->text().toInt();

  if (currentPass == Data::getPassword())
    {
      int firstInputPass = ui->newPasswordLineEdit->text().toInt();
      int secondInputPass = ui->retypePasswordLineEdit->text().toInt();

      if (firstInputPass == secondInputPass)
        {
           Data::changePassword(secondInputPass);
           QMessageBox message(this);
           message.setText("Password changed sucessfully.");
           message.setDefaultButton(QMessageBox::Ok);
           message.exec();
           accept();
        }
      else
        {
          QMessageBox message(this);
          message.setText("The input passwords do not match.");
          message.setDefaultButton(QMessageBox::Ok);
          message.exec();
        }
    }
  else
    {
      QMessageBox message(this);
      message.setText("Current password is incorrect.");
      message.setDefaultButton(QMessageBox::Ok);
      message.exec();
      return;
    }
}

void ChangePasswordDialog::on_cancelButton_clicked()
{
  reject();
}

void ChangePasswordDialog::setFocusWidget()
{
  lastWidget = focusWidget();
  ui->numpadWidget->setSetWidget(lastWidget);

}

void ChangePasswordDialog::onRemoveButtonClicked()
{
  if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(lastWidget))
    {
      QString text = lineEdit->text();
      text.remove(text.size() - 1, 1);
      lineEdit->setText(text);
    }
}

void ChangePasswordDialog::clearInputs()
{
  ui->curentPasswordLineEdit->clear();
  ui->newPasswordLineEdit->clear();
  ui->retypePasswordLineEdit->clear();
}

void ChangePasswordDialog::showEvent(QShowEvent *event)
{
  clearInputs();
}
