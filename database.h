#ifndef DATABASE_H
#define DATABASE_H


#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSettings>

class Database {
 public:
    static QSqlDatabase db;
    static bool connect(){
        QSettings settings("config.ini", QSettings::IniFormat);

        QString type = settings.value("database/type,QMYSQL").toString();
        QString host = settings.value("database/host","127.0.0.1").toString();
        int port = settings.value("database/port",3306).toInt();
        QString name = settings.value("database/name","projetdatabase").toString();
        QString user = settings.value("database/user","dinart_remote").toString();
        QString password = settings.value("database/password","Deralaza").toString();
        db= QSqlDatabase::addDatabase(type);
        db.setHostName(host);
        db.setPort(port);
        db.setDatabaseName(name);
        db.setUserName(user);
        db.setPassword(password);
        if(!db.open()){
            qDebug() << "Erreur de connexion BD:" << db.lastError().text();
            return false;
        }
        qDebug() << "conexion reussie a la BD sur" << host;
        return true;
    }
};
#endif // DATABASE_H

