#ifndef LOCKSCREENFORM_H
#define LOCKSCREENFORM_H

#include <QWidget>
#include <QIntValidator>
#include <QShowEvent>
#include <QTimer>

namespace Ui {
  class LockScreenForm;
}

class LockScreenForm : public QWidget
{
  Q_OBJECT

public:
  explicit LockScreenForm(QWidget *parent = 0);
  ~LockScreenForm();


signals:
  void loginPassed();

private slots:
  void onOkbuttonClicked();
  void onRemoveButtonClicked();
  void hideAllWidgets();
  void showAllWidgets();

private:
  void setBackgroundImage();
  QIntValidator intValidator;

  Ui::LockScreenForm *ui;

  // QWidget interface
protected:
  virtual void showEvent(QShowEvent *event);
  QTimer hideWidgetsTimer;

  // QWidget interface
protected:
  virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // LOCKSCREENFORM_H

