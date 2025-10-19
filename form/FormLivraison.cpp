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

void FormLivraison::mettreAJourStock(const QString &ref_Mar, const QString &ref_entrepot, int qte_stock)
{
    QSqlQuery query(Database::db);
    query.prepare("SELECT qte_stock FROM stocker WHERE ref_mar = ? AND ref_entrepot = ?");
    query.addBindValue(ref_Mar);
    query.addBindValue(ref_entrepot);

    if(query.exec() && query.next()){
        int qteExistante = query.value(0).toInt();
        int qteNouvelle = qteExistante + qte_stock;

        QSqlQuery updateQuery(Database::db);
        updateQuery.prepare("UPDATE stocker SET qte_stock = ? WHERE ref_mar = ? AND ref_entrepot = ?");
        updateQuery.addBindValue(qteNouvelle);
        updateQuery.addBindValue(ref_Mar);
        updateQuery.addBindValue(ref_entrepot);
        updateQuery.exec();
    } else {
        QSqlQuery insertQuery(Database::db);
        insertQuery.prepare("INSERT INTO stocker(num_mar, ref_entr, qte_stock) VALUES (?, ?, ?)");
        insertQuery.addBindValue(ref_Mar);
        insertQuery.addBindValue(ref_entrepot);
        insertQuery.addBindValue(qte_stock);
        insertQuery.exec();
    }
}

void FormLivraison::on_btnAjouter_clicked()
{
    QString ref_frs = ui->lineRefFrs->text();       // CORRIGÉ
    QString ref_Mar = ui->lineNumMar->text();       // CORRIGÉ
    QString ref_entrepot = ui->lineRefEntr->text();     // CORRIGÉ
    QDate date = ui->dateEdit->date();
    double prix_achat = ui->linePrix->text().toDouble();
    int qte_stock = ui->lineQte->text().toInt();

    QSqlQuery query(Database::db);
    query.prepare("INSERT INTO livrer(ref_frs, ref_mar, ref_entrepot, date_livraison, prix_achat, qte_livree) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(ref_frs);
    query.addBindValue(ref_Mar);
    query.addBindValue(ref_entrepot);
    query.addBindValue(date);
    query.addBindValue(prix_achat);
    query.addBindValue(qte_stock);

    if(!query.exec()){
        QMessageBox::warning(this,"Erreur","Impossible d'ajouter la livraison:\n"+query.lastError().text());
        return;
    }

    mettreAJourStock(ref_Mar, ref_entrepot, qte_stock);
    QMessageBox::information(this, "Succès", "La livraison a été ajoutée et le stock mis à jour.");
    this->accept();
}
