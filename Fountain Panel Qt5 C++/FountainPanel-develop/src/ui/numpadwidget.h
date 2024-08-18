#ifndef NUMPADWIDGET_H
#define NUMPADWIDGET_H

#include <QWidget>

namespace Ui {
  class NumpadWidget;
}

class NumpadWidget : public QWidget
{
  Q_OBJECT

public:
  explicit NumpadWidget(QWidget *parent = 0);
  ~NumpadWidget();

  void setSetWidget(QWidget *value);

signals:
  void okButtonClicked();
  void removeButtonClicked();

private slots:
  void on_removeButton_clicked();

  void on_num1Button_clicked();

  void on_num2Button_clicked();

  void on_num3Button_clicked();

  void on_num4Button_clicked();

  void on_num5Button_clicked();

  void on_num6Button_clicked();

  void on_num7Button_clicked();

  void on_num8Button_clicked();

  void on_num9Button_clicked();

  void on_num0Button_clicked();

  void on_okButton_clicked();
  // QWidget interface
protected:
  virtual void showEvent(QShowEvent *);
private:
  void addNumberToFocusedWidget(const int &number);

  Ui::NumpadWidget *ui;

  QWidget* setWidget;
};

#endif // NUMPADWIDGET_H
