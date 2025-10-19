#include "model/stock.h"
#include "database/database.h"
#include <QDebug>
#include <QSqlError>

bool Stock::mettreAjour(QString ref_Mar, QString ref_Entrepot, int qte_Livree){
    QSqlQuery query(Database::db);
    query.prepare("SELECT qte_stock FROM stocker WHERE ref_mar = ? AND ref_entrepot = ? ");
    query.addBindValue(ref_Mar);
    query.addBindValue(ref_Entrepot);

    if(!query.exec()){
        qDebug() << "Erreur verification stock:" << query.lastError().text();
        return false;
    }
    if(query.next()){
        int qteExistante = query.value(0).toInt();
        int nouvelleQte = qteExistante + qte_Livree;

        QSqlQuery updateQuery(Database::db);
        updateQuery.prepare("UPDATE stocker SET qte_stock = ? WHERE ref_mar = ? AND ref_entrepot = ?");
        updateQuery.addBindValue(nouvelleQte);
        updateQuery.addBindValue(ref_Mar);
        updateQuery.addBindValue(ref_Entrepot);
              if(!updateQuery.exec()){
                  qDebug() << "Erreur mise a jour stock :" << updateQuery.lastError().text();
                  return false;
              }
              qDebug() << "Stock mis a jour avec succes";
    }
      else {
        QSqlQuery insertQuery(Database::db);
        insertQuery.prepare("INSERT INTO stocker(ref_mar , ref_entrepot , qt_stock) VALUES (?, ?, ?) ");
        insertQuery.addBindValue(ref_Mar);
        insertQuery.addBindValue(ref_Entrepot);
        insertQuery.addBindValue(qte_Livree);
           if(!insertQuery.exec()) {
               qDebug() << "Erreur insertion stock:" << insertQuery.lastError().text();
               return false;
           }

           qDebug() << "Nouveau stock ajouter avec succes!";
    }
    return true;
}
