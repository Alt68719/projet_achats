#include "FormFournisseur.h"
#include "ui_FormFournisseur.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

FormFournisseur::FormFournisseur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormFournisseur)
{
    ui->setupUi(this);
}

FormFournisseur::~FormFournisseur()
{
    delete ui;
}

void FormFournisseur::on_btnAjouter_clicked()
{
    QString ref = ui->lineEditRef->text();          // CORRIGÉ
    QString raison = ui->lineEditRaison->text();    // CORRIGÉ
    QString nif = ui->lineEditNIF->text();         // CORRIGÉ
    QString adresse = ui->lineEditAdresse->text();  // CORRIGÉ
    QString tel = ui->lineEditTel->text();          // CORRIGÉ

    QSqlQuery query(Database::db);
    query.prepare("INSERT INTO fournisseur(ref_frs, raison_sociale, nif, adresse, tel) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(ref);
    query.addBindValue(raison);
    query.addBindValue(nif);
    query.addBindValue(adresse);
    query.addBindValue(tel);

    if(!query.exec()){
        QMessageBox::warning(this,"Erreur","Impossible d'ajouter le fournisseur:\n"+query.lastError().text());
        return;
    }

    QMessageBox::information(this,"Succès","Fournisseur ajouté !");
    accept();
}
