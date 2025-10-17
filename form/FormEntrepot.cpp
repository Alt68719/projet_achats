#include "FormEntrepot.h"
#include "ui_FormEntrepot.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

FormEntrepot::FormEntrepot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormEntrepot)
{
    ui->setupUi(this);
}

FormEntrepot::~FormEntrepot()
{
    delete ui;
}

void FormEntrepot::on_btnAjouter_clicked()
{
    QString ref = ui->lineEditRef->text();        // CORRIGÉ
    QString adresse = ui->lineEditAdresse->text(); // CORRIGÉ

    QSqlQuery query(Database::db);
    query.prepare("INSERT INTO entrepot(ref_entr, adresse) VALUES (?, ?)");
    query.addBindValue(ref);
    query.addBindValue(adresse);

    if(!query.exec()){
        QMessageBox::warning(this,"Erreur","Impossible d'ajouter l'entrepôt:\n"+query.lastError().text());
        return;
    }

    QMessageBox::information(this,"Succès","Entrepôt ajouté !");
    accept();
}
