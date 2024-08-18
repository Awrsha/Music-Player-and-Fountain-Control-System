#ifndef ADDTOPLAYLISTDIALOG_H
#define ADDTOPLAYLISTDIALOG_H

#include <QDialog>
#include <QMessageBox>

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

private slots:
  void on_selectMusicButton_clicked();

  void on_selectFountainButton_clicked();

  void on_addButton_clicked();

  void on_cancelButton_clicked();

private:

  bool isBothEmpty();
  Ui::AddToPlaylistDialog *ui;

  QString musicAddress;
  QString fountainAddress;
};

#endif // ADDTOPLAYLISTDIALOG_H
