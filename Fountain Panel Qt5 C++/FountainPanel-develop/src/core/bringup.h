#ifndef BRINGUP_H
#define BRINGUP_H

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QTextStream>


class BringUp
{
public:
  BringUp();

  static void setApplicationFont(QApplication &app);

  static void setApplicationStyle(QApplication &app);

  static void loadVirtualKeyboard();

  static void loadApplicationTheme(QString& applicationTheme, const bool &isDark);
};

#endif // BRINGUP_H
