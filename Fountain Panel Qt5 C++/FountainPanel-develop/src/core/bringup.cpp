#include "bringup.h"
#include <QDebug>
#include "src/util/projecttheme.h"

BringUp::BringUp()
{

}

void BringUp::setApplicationFont(QApplication& app)
{

  QFontDatabase::addApplicationFont(":/res/fonts/OpenSans-Light.ttf");
  QFontDatabase::addApplicationFont(":/res/fonts/IranSans.ttf");
  QFontDatabase::addApplicationFont(":/res/fonts/DroidNaskh-Regular.ttf");

  qApp->setFont(QFont("Open Sans", 14));
//  qApp->setFont(QFont("IRAN Sans", 9));
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
      QString textFile = ts.readAll();

      loadApplicationTheme(textFile, true);

      app.setStyleSheet(textFile);
    }

//  app.setOverrideCursor(Qt::BlankCursor);

}

void BringUp::loadVirtualKeyboard()
{
  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
}

void BringUp::loadApplicationTheme(QString &applicationTheme, const bool& isDark)
{
  if (isDark)
    {
      applicationTheme.replace(ProjectTheme::ColorText::PRIMERY_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::ACCENT_COLOR_TEXT, ProjectTheme::Colors::ACCENT_COLD_NORMAL.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::PRIMARY_TEXT_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_TEXT_DARK.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::SECONDARY_TEXT_COLOR_TEXT, ProjectTheme::Colors::SECONDARY_TEXT_DARK.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::DISABLED_TEXT_COLOR_TEXT, ProjectTheme::Colors::DISABLED_TEXT_DARK.name(QColor::HexArgb));

//      applicationTheme.replace(ProjectTheme::ColorText::STATUS_BAR_COLOR_TEXT, ProjectTheme::Colors::STATUS_BAR_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::APP_BAR_COLOR_TEXT, ProjectTheme::Colors::APP_BAR_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::DIALOGS_COLOR_TEXT, ProjectTheme::Colors::DIALOGS_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_DARK.name(QColor::HexArgb));


      applicationTheme.replace(ProjectTheme::ColorText::STATUS_BAR_COLOR_TEXT, ProjectTheme::Colors::STATUS_BAR_DARK.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::APP_BAR_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_DARK.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::DIALOGS_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_NORMAL_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_NORMAL_DARK.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_FOCUSED_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_FOCUSED_DARK.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_PRESSED_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_PRESSED_DARK.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_DISABLED_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_DISABLED_DARK.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::DIVIDER_COLOR_TEXT, ProjectTheme::Colors::DIVIDER_DARK.name(QColor::HexArgb));

//      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_1_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_2_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_3_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_4_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_5_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_DARK.name(QColor::HexArgb));
//      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_6_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_DARK.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_1_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_2_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_3_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_4_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_5_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_6_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));


    }
  else
    {
      applicationTheme.replace(ProjectTheme::ColorText::PRIMERY_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_COLOR_NORMAL.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::ACCENT_COLOR_TEXT, ProjectTheme::Colors::ACCENT_COLD_NORMAL.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::PRIMARY_TEXT_COLOR_TEXT, ProjectTheme::Colors::PRIMARY_TEXT_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::SECONDARY_TEXT_COLOR_TEXT, ProjectTheme::Colors::SECONDARY_TEXT_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::DISABLED_TEXT_COLOR_TEXT, ProjectTheme::Colors::DISABLED_TEXT_LIGHT.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::STATUS_BAR_COLOR_TEXT, ProjectTheme::Colors::STATUS_BAR_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::APP_BAR_COLOR_TEXT, ProjectTheme::Colors::APP_BAR_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::DIALOGS_COLOR_TEXT, ProjectTheme::Colors::DIALOGS_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_LIGHT.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_NORMAL_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_NORMAL_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_FOCUSED_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_FOCUSED_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_PRESSED_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_PRESSED_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BUTTON_BACKGROUND_DISABLED_TEXT, ProjectTheme::Colors::BUTTON_BACKGROUND_DISABLED_LIGHT.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::DIVIDER_COLOR_TEXT, ProjectTheme::Colors::DIVIDER_LIGHT.name(QColor::HexArgb));

      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_1_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_2_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_3_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_4_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_5_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_LIGHT.name(QColor::HexArgb));
      applicationTheme.replace(ProjectTheme::ColorText::BACKGROUND_6_COLOR_TEXT, ProjectTheme::Colors::BACKGROUND_LIGHT.name(QColor::HexArgb));
    }
}
