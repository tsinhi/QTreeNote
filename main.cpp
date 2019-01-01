#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setApplicationName(QObject::tr("QTreeNote"));

    QIcon appIcon;
    appIcon.addPixmap(QPixmap(":/icons/QTreeNote.png"));
    QApplication::setWindowIcon(appIcon);

    MainWindow w;
    w.show();

    return a.exec();
}
