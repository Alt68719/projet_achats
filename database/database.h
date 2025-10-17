#pragma once
#include <QSqlDatabase>

class Database {
public:
    static QSqlDatabase db;
    static bool ouvrir();
    static void fermer();
};
