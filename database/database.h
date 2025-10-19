#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>
#pragma once
#include <QSqlDatabase>
#include <QString>

class Database {
public:
    static QSqlDatabase db;
    static bool ouvrir();
    static void fermer();
    static void creerTables();
    static const QString SQL_SCHEMA;
};
#endif
