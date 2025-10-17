#ifndef STOCK_H
#define STOCK_H

#endif // STOCK_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
class Stock {
    static bool mettreAjour(QString refMAr , QString refEntr , int qteLivree);
    static QSqlQueryModel* inventaire(QDate date);
};
