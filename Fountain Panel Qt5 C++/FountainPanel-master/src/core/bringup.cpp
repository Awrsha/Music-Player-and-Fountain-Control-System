#include "bringup.h"
#include <QDebug>

BringUp::BringUp()
{

}

void BringUp::setApplicationFont(QApplication& app)
{

  QFontDatabase::addApplicationFont(":/res/fonts/OpenSans-Light.ttf");
  QFontDatabase::addApplicationFont(":/res/fonts/IranSans.ttf");
  QFontDatabase::addApplicationFont(":/res/fonts/DroidNaskh-Regular.ttf");

//  qApp->setFont(QFont("Open Sans", 9));
  qApp->setFont(QFont("IRAN Sans", 9));
//  qApp->setFont(QFont("Droid Arabic Naskh", 9));

}

void BringUp::setApplicationStyle(QApplication& app)
{
  QFile f(":qdarkstyle/style.qss");

  if (!f.exists())
    {
      qWarning()<<"Unable to set stylesheet, file not found\n";
    }
  else
    {
      f.open(QFile::ReadOnly | QFile::Text);
      QTextStream ts(&f);
      app.setStyleSheet(ts.readAll());
    }

//  app.setOverrideCursor(Qt::BlankCursor);

}
