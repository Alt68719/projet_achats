#include "model/stock.h"
#include "database/database.h"
#include <QDebug>
#include <QSqlError>

bool Stock::mettreAjour(QString refMar, QString refEntr, int qteLivree){
    QSqlQuery query(Database::db);
    query.prepare("SELECT qte_stock FROM stocker WHERE ref_mar = ? AND ref_entrepot = ? ");
    query.addBindValue(refMar);
    query.addBindValue(refEntr);

    if(!query.exec()){
        qDebug() << "Erreur verification stock:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        int qteExistante = query.value(0).toInt();
        int nouvelleQte = qteExistante + qteLivree;

        QSqlQuery updateQuery(Database::db);
        updateQuery.prepare("UPDATE stocker SET qte_stock = ? WHERE ref_mar = ? AND ref_entrepot = ?");
        updateQuery.addBindValue(nouvelleQte);
        updateQuery.addBindValue(refMar);
        updateQuery.addBindValue(refEntr);
              if(!updateQuery.exec()){
                  qDebug() << "Erreur mise a jour stock :" << updateQuery.lastError().text();
                  return false;
              }
              qDebug() << "Stock mis a jour avec succes";
    }
      else {
        QSqlQuery insertQuery(Database::db);
        insertQuery.prepare("INSERT INTO stocker(ref_mar , ref_entrepot , qt_stock) VALUES (?, ?, ?) ");
        insertQuery.addBindValue(refMar);
        insertQuery.addBindValue(refEntr);
        insertQuery.addBindValue(qteLivree);
           if(!insertQuery.exec()) {
               qDebug() << "Erreur insertion stock:" << insertQuery.lastError().text();
               return false;
           }

           qDebug() << "Nouveau stock ajouter avec succes!";
    }
    return true;
}
