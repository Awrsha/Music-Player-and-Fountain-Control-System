#include "lockscreenform.h"
#include "ui_lockscreenform.h"
#include <QMessageBox>
#include "src/core/data.h"
#include <QDirIterator>


LockScreenForm::LockScreenForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LockScreenForm)
{
  ui->setupUi(this);

  ui->lineEdit->setValidator(&intValidator);

  hideWidgetsTimer.setInterval(10000);
  hideWidgetsTimer.setSingleShot(true);
  hideWidgetsTimer.start();

  connect(&hideWidgetsTimer, SIGNAL(timeout()),
          SLOT(hideAllWidgets()));

  connect(ui->numpadWidget, SIGNAL(okButtonClicked()),
          SLOT(onOkbuttonClicked()));

  connect(ui->numpadWidget, SIGNAL(removeButtonClicked()),
          SLOT(onRemoveButtonClicked()));
  ui->numpadWidget->setSetWidget(/*(QWidget*)*/ui->lineEdit);

}

LockScreenForm::~LockScreenForm()
{
  delete ui;
}


void LockScreenForm::showEvent(QShowEvent *event)
{
  ui->lineEdit->clear();
  setBackgroundImage();
}

void LockScreenForm::mouseReleaseEvent(QMouseEvent *event)
{
  showAllWidgets();
  hideWidgetsTimer.start();
}




void LockScreenForm::onOkbuttonClicked()
{
  if (ui->lineEdit->text().isEmpty())
    {
      QMessageBox message(this);
      message.setText("Please enter the password.");
      message.setDefaultButton(QMessageBox::Ok);
      message.exec();
      return;
    }

  int currentPass = ui->lineEdit->text().toInt();

  if (currentPass == Data::getPassword())
    {
      emit loginPassed();
    }
  else
    {
      QMessageBox message(this);
      message.setText("The password is incorrect, try again.");
      message.setDefaultButton(QMessageBox::Ok);
      message.exec();
      ui->lineEdit->clear();
      return;
    }
}

void LockScreenForm::onRemoveButtonClicked()
{
  if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(ui->lineEdit))
    {
      QString text = lineEdit->text();
      text.remove(text.size() - 1, 1);
      lineEdit->setText(text);
    }
}

void LockScreenForm::hideAllWidgets()
{
  foreach (QWidget* widget , findChildren<QWidget*>()) {
      if (widget->isVisible())
        {
          widget->setVisible(false);
        }
    }
}

void LockScreenForm::showAllWidgets()
{
  foreach (QWidget* widget , findChildren<QWidget*>()) {
      if (!widget->isVisible())
        {
          widget->setVisible(true);
        }
    }
}

void LockScreenForm::setBackgroundImage()
{

  QDir backgroundImageDir("./background");
  QVector<QString> imageList;

  if (backgroundImageDir.exists())
    {

      QDirIterator it(backgroundImageDir.absolutePath(), QStringList() << "*.jpg", QDir::Files, QDirIterator::Subdirectories);
      while (it.hasNext())
        {
          imageList.push_back(it.next());
        }
    }

  if (!imageList.isEmpty())
    {
      QString image = imageList.at(qrand() % imageList.size());
      QPixmap background(image);
      background = background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
      QPalette palette;
      palette.setBrush(QPalette::Background, background);
      setPalette(palette);
      setAutoFillBackground(true);
    }
}




