#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QShowEvent>
#include <QIntValidator>

namespace Ui {
  class ChangePasswordDialog;
}

class ChangePasswordDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ChangePasswordDialog(QWidget *parent = 0);
  ~ChangePasswordDialog();

private slots:
  void on_changePasswordButton_clicked();

  void on_cancelButton_clicked();

  void setFocusWidget();
  void onRemoveButtonClicked();
private:
  Ui::ChangePasswordDialog *ui;
  void clearInputs();
  QIntValidator intValidator;

  // QWidget interface
protected:
  virtual void showEvent(QShowEvent *event);

  QWidget* lastWidget;
};

#endif // CHANGEPASSWORDDIALOG_H
