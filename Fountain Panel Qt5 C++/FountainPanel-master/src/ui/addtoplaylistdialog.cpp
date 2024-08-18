#include "addtoplaylistdialog.h"
#include "ui_addtoplaylistdialog.h"
#include <QFileDialog>

AddToPlaylistDialog::AddToPlaylistDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddToPlaylistDialog)
{
  ui->setupUi(this);
}

AddToPlaylistDialog::~AddToPlaylistDialog()
{
  delete ui;
}

void AddToPlaylistDialog::on_selectMusicButton_clicked()
{
  ui->musicLineEdit->setText(QFileDialog::getOpenFileName(
                               this,
                               tr("Add"),
                               ".",
                               tr("MP3 Audio files(*.mp1 *.mp2 *.mp3);; WMA Audio files(*.wma)")));
}

void AddToPlaylistDialog::on_selectFountainButton_clicked()
{
  ui->fountainLineEdit->setText(QFileDialog::getOpenFileName(
                               this,
                               tr("Add"),
                               ".",
                               tr("Water Show Exported files(*.bin)")));
}

void AddToPlaylistDialog::on_addButton_clicked()
{
  if (isBothEmpty())
    {
      QMessageBox message(this);
      message.setText("Please Select a Music or Fountain File");
      message.setDefaultButton(QMessageBox::Ok);
      message.exec();
      return;
    }

  fountainAddress = ui->fountainLineEdit->text();
  musicAddress = ui->musicLineEdit->text();

  accept();
}

void AddToPlaylistDialog::on_cancelButton_clicked()
{
  reject();
}

bool AddToPlaylistDialog::isBothEmpty()
{
  return ui->musicLineEdit->text().isEmpty() && ui->fountainLineEdit->text().isEmpty();
}

QString AddToPlaylistDialog::getFountainAddress() const
{
  return fountainAddress;
}

QString AddToPlaylistDialog::getMusicAddress() const
{
  return musicAddress;
}
