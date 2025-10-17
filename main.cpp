#include "mainwindow.h"
#include <QApplication>
#include "database/database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!Database::ouvrir()) {
        return -1; // Impossible d’ouvrir la base
    }

    MainWindow w;
    w.show();
    return a.exec();
}
