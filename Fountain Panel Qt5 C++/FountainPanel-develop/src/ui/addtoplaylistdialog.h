#ifndef ADDTOPLAYLISTDIALOG_H
#define ADDTOPLAYLISTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "src/core/mediainfo.h"
#include <QFileDialog>

namespace Ui {
  class AddToPlaylistDialog;
}

class AddToPlaylistDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AddToPlaylistDialog(QWidget *parent = 0);
  ~AddToPlaylistDialog();

  QString getMusicAddress() const;

  QString getFountainAddress() const;

  MusicFountainInfo::WorkMode getDayNightMode() const;
  MusicFountainInfo::WorkMode getNoWindWindMode() const;

private slots:
  void on_selectMusicButton_clicked();

  void on_selectFountainButton_clicked();

  void on_addButton_clicked();

  void on_cancelButton_clicked();

  void onHandleDayNightComboChange(const int& index);
  void onHandleWindNoWindComboChange(const int& index);



private:
  void initGui();
  bool isBothEmpty();
  Ui::AddToPlaylistDialog *ui;

  QFileDialog* fountainFileDialog;
  QFileDialog* musicFileDialog;

  QString musicAddress;
  QString fountainAddress;
};

#endif // ADDTOPLAYLISTDIALOG_H
