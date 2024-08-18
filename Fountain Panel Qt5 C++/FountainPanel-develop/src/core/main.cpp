#include "src/ui/mainwindow.h"
#include <QApplication>

#include "src/core/bringup.h"

int main(int argc, char *argv[])
{ 

//  BringUp::loadVirtualKeyboard();
  QApplication app(argc, argv);

  BringUp::setApplicationStyle(app);
  BringUp::setApplicationFont(app);


  app.processEvents();

  MainWindow w;
  w.show/*FullScreen*/();

  return app.exec();
}
