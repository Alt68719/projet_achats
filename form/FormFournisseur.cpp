#include "form/FormFournisseur.h"
#include "ui_FormFournisseur.h"
#include "database/database.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableView>
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

// ... (le reste des includes) ...

void FormFournisseur::on_btnAjouter_clicked()
{
    QString ref_frs = ui->lineEditRef->text();
    QString raison_sociale = ui->lineEditRaison->text();

    // 🚨 Correction : Convertir NIF et Tel_frs en int
    // La méthode .toInt() tente la conversion. Si l'utilisateur entre du texte, elle renvoie 0.
    int NIF = ui->lineEditNIF->text().toInt();
    int tel_frs = ui->lineEditTel->text().toInt();

    QString adresse = ui->lineEditAdresse->text();

    QSqlQuery query(Database::db);

    // 🚨 Correction : La colonne du téléphone est 'tel_frs' dans votre schéma,
    // mais dans la requête vous utilisez 'tel'. Il faut uniformiser.
    // D'après votre schéma (database.cpp), la colonne est 'tel_frs'.
    query.prepare("INSERT INTO fournisseur(ref_frs, raison_sociale, NIF, adresse, tel_frs) VALUES (?, ?, ?, ?, ?)");

    query.addBindValue(ref_frs);
    query.addBindValue(raison_sociale);
    query.addBindValue(NIF);     // BindValue gérera le type int
    query.addBindValue(adresse);
    query.addBindValue(tel_frs); // BindValue gérera le type int

    if(!query.exec()){
        QMessageBox::warning(this,"Erreur","Impossible d'ajouter le fournisseur:\n"+query.lastError().text());
        return;
    }

    QMessageBox::information(this,"Succès","Fournisseur ajouté !");
    accept();
}
