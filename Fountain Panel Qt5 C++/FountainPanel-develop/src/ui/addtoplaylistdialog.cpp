#include "addtoplaylistdialog.h"
#include "ui_addtoplaylistdialog.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <qdebug.h>

#include "src/util/projecttheme.h"

AddToPlaylistDialog::AddToPlaylistDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddToPlaylistDialog)
{
  ui->setupUi(this);
  initGui();
}

AddToPlaylistDialog::~AddToPlaylistDialog()
{
  delete ui;
}

void AddToPlaylistDialog::on_selectMusicButton_clicked()
{
//<<<<<<< HEAD
//  ui->musicLineEdit->setText(QFileDialog::getOpenFileName(
//                               this,
//                               tr("Add"),
//                               "/home/pi/Music",
//                               tr("MP3 Audio files(*.mp1 *.mp2 *.mp3);; WMA Audio files(*.wma)")));
//=======

  //  QList<QUrl> urls;
  //  urls << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

  //  urls.append(QUrl::fromLocalFile("/home/ar/Music"));
  //  urls.append(QUrl::fromLocalFile("/home/ar/Pictures"));
  //  urls.append(QUrl::fromLocalFile("/home/ar/Downloads"));


  //  QFileDialog* fileDialog = new QFileDialog(this, "Add", ".", "MP3 Audio files(*.mp1 *.mp2 *.mp3);; WMA Audio files(*.wma)" );

  //  fileDialog->setSidebarUrls(urls);

  musicFileDialog->selectFile("");
  if (musicFileDialog->exec() == QDialog::Accepted)
    {
      if (!musicFileDialog->selectedFiles().empty())
        {
          ui->musicLineEdit->setText(musicFileDialog->selectedFiles().front());
        }
    }
//>>>>>>> 20c61b5c2ba84cea166a6cade11bcd6759fb540d
}

void AddToPlaylistDialog::on_selectFountainButton_clicked()
{
//<<<<<<< HEAD
//  ui->fountainLineEdit->setText(QFileDialog::getOpenFileName(
//                               this,
//                               tr("Add"),
//                               "/home/pi/Music",
//                               tr("Water Show Exported files(*.bin)")));
//=======
  //  QList<QUrl> urls;
  //  urls << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());

  //  urls.append(QUrl::fromLocalFile("/home/ar/Music"));
  //  urls.append(QUrl::fromLocalFile("/home/ar/Pictures"));
  //  urls.append(QUrl::fromLocalFile("/home/ar/Downloads"));


  //  QFileDialog fileDialog(this, "Add", ".", "Water Show Exported files(*.bin)" );

  //  fileDialog.setSidebarUrls(urls);

  fountainFileDialog->selectFile("");
  if (fountainFileDialog->exec() == QDialog::Accepted)
    {
      ui->fountainLineEdit->setText(fountainFileDialog->selectedFiles().front());
    }

//>>>>>>> 20c61b5c2ba84cea166a6cade11bcd6759fb540d
}

void AddToPlaylistDialog::on_addButton_clicked()
{
  qDebug()<<"add";
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


  ui->fountainLineEdit->clear();
  ui->musicLineEdit->clear();
  accept();
}

void AddToPlaylistDialog::on_cancelButton_clicked()
{
  ui->fountainLineEdit->clear();
  ui->musicLineEdit->clear();
  reject();
}

void AddToPlaylistDialog::onHandleDayNightComboChange(const int &index)
{

  QPixmap pixmap;
  if (index == 0)
    {
      pixmap.load(ProjectTheme::Resource::ICON_ONLY_DAY);
    }
  else if (index == 1)
    {
      pixmap.load(ProjectTheme::Resource::ICON_ONLY_NIGHT);
    }
  else if (index == 2)
    {
      pixmap.load(ProjectTheme::Resource::ICON_DAY_AND_NIGHT);
    }

  ui->dayNightViewLabel->setPixmap(pixmap);
}

void AddToPlaylistDialog::onHandleWindNoWindComboChange(const int &index)
{
  QPixmap pixmap;
  if (index == 0)
    {
      pixmap.load(ProjectTheme::Resource::ICON_NO_WIND);
    }
  else if (index == 1)
    {
      pixmap.load(ProjectTheme::Resource::ICON_WIND);
    }
  else if (index == 2)
    {
      pixmap.load(ProjectTheme::Resource::ICON_WIND_AND_NO_WIND);
    }

  ui->windNoWindViewLabel->setPixmap(pixmap);
}

MusicFountainInfo::WorkMode AddToPlaylistDialog::getDayNightMode() const
{
  return (MusicFountainInfo::WorkMode)ui->dayNightCombo->currentIndex();
}

MusicFountainInfo::WorkMode AddToPlaylistDialog::getNoWindWindMode() const
{
  return (MusicFountainInfo::WorkMode)ui->windNoWindCombo->currentIndex();
}

void AddToPlaylistDialog::initGui()
{
  setWindowFlags(Qt::FramelessWindowHint);

  QStringList dayNight;
  dayNight << "Only Day" << "Only Night" << "Day and Night";
  ui->dayNightCombo->addItems(dayNight);

  QStringList windNoWind;
  windNoWind << "No Wind" << "Wind" << "Wind and No Wind";
  ui->windNoWindCombo->addItems(windNoWind);

  connect(ui->dayNightCombo, SIGNAL(currentIndexChanged(int)),
          SLOT(onHandleDayNightComboChange(int)));

  connect(ui->windNoWindCombo, SIGNAL(currentIndexChanged(int)),
          SLOT(onHandleWindNoWindComboChange(int)));

  QList<QUrl> urls;
  urls << QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());


  musicFileDialog = new QFileDialog(this, "Add", ".", "MP3 Audio files(*.mp1 *.mp2 *.mp3);; WMA Audio files(*.wma)" );
  fountainFileDialog = new QFileDialog(this, "Add", ".", "Water Show Exported files(*.bin)" );

  musicFileDialog->setSidebarUrls(urls);
  fountainFileDialog->setSidebarUrls(urls);


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
