#include "FormLivraison.h"
#include "ui_FormLivraison.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

FormLivraison::FormLivraison(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLivraison)
{
    ui->setupUi(this);
}

FormLivraison::~FormLivraison()
{
    delete ui;
}

void FormLivraison::mettreAJourStock(const QString &numMar, const QString &refEntr, int qte)
{
    QSqlQuery query(Database::db);
    query.prepare("SELECT qte_stock FROM stocker WHERE num_mar = ? AND ref_entr = ?");
    query.addBindValue(numMar);
    query.addBindValue(refEntr);

    if(query.exec() && query.next()){
        int qteExistante = query.value(0).toInt();
        int qteNouvelle = qteExistante + qte;

        QSqlQuery updateQuery(Database::db);
        updateQuery.prepare("UPDATE stocker SET qte_stock = ? WHERE num_mar = ? AND ref_entr = ?");
        updateQuery.addBindValue(qteNouvelle);
        updateQuery.addBindValue(numMar);
        updateQuery.addBindValue(refEntr);
        updateQuery.exec();
    } else {
        QSqlQuery insertQuery(Database::db);
        insertQuery.prepare("INSERT INTO stocker(num_mar, ref_entr, qte_stock) VALUES (?, ?, ?)");
        insertQuery.addBindValue(numMar);
        insertQuery.addBindValue(refEntr);
        insertQuery.addBindValue(qte);
        insertQuery.exec();
    }
}

void FormLivraison::on_btnAjouter_clicked()
{
    QString refFrs = ui->lineRefFrs->text();       // CORRIGÉ
    QString numMar = ui->lineNumMar->text();       // CORRIGÉ
    QString refEntr = ui->lineRefEntr->text();     // CORRIGÉ
    QDate date = ui->dateEdit->date();
    double prix = ui->linePrix->text().toDouble();
    int qte = ui->lineQte->text().toInt();

    QSqlQuery query(Database::db);
    query.prepare("INSERT INTO livrer(ref_frs, num_mar, ref_entr, date_livraison, prix_achat, qte_livree) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(refFrs);
    query.addBindValue(numMar);
    query.addBindValue(refEntr);
    query.addBindValue(date);
    query.addBindValue(prix);
    query.addBindValue(qte);

    if(!query.exec()){
        QMessageBox::warning(this,"Erreur","Impossible d'ajouter la livraison:\n"+query.lastError().text());
        return;
    }

    mettreAJourStock(numMar
