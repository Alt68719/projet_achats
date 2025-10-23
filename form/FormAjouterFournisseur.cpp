#include "FormAjouterFournisseur.h"
#include "ui_FormAjouterFournisseur.h" // Le fichier généré par le nouveau UI
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>

FormAjouterFournisseur::FormAjouterFournisseur(QSqlTableModel *modelFrs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormAjouterFournisseur),
    modelFournisseur(modelFrs) // Stocke le pointeur du modèle principal
{
    ui->setupUi(this);
}

FormAjouterFournisseur::~FormAjouterFournisseur()
{
    delete ui;
}

// Opération CREATE: Ajouter un nouveau fournisseur
void FormAjouterFournisseur::on_btnAjouter_clicked()
{
    QString ref = ui->lineEditRefFrs->text().trimmed();
    QString raisonSociale = ui->lineEditRaisonSociale->text().trimmed();
    // Assurez-vous que NIF et Téléphone sont des entiers avant de les convertir
    bool nifOk, telOk;
    int NIF = ui->lineEditNIF->text().trimmed().toInt(&nifOk);
    int tel = ui->lineEditTelFrs->text().trimmed().toInt(&telOk);
    QString adresse = ui->lineEditAdresse->text().trimmed();

    if (ref.isEmpty() || raisonSociale.isEmpty() || adresse.isEmpty() || !nifOk || !telOk) {
        QMessageBox::warning(this, "Erreur de Saisie", "Veuillez remplir tous les champs correctement (NIF et Tél doivent être des nombres).");
        return;
    }

    QSqlRecord newRecord = modelFournisseur->record();
    newRecord.setValue("ref_frs", ref);
    newRecord.setValue("raison_sociale", raisonSociale);
    newRecord.setValue("NIF", NIF);
    newRecord.setValue("adresse", adresse);
    newRecord.setValue("tel_frs", tel);

    if (!modelFournisseur->insertRecord(-1, newRecord)) {
        QMessageBox::critical(this, "Erreur SQL (Insertion)", "Impossible d'ajouter le fournisseur : " + modelFournisseur->lastError().text());
        modelFournisseur->revertAll();
        return;
    }

    if (!modelFournisseur->submitAll()) {
        QMessageBox::critical(this, "Erreur SQL (Sauvegarde)", "Échec de la sauvegarde en base : " + modelFournisseur->lastError().text());
        modelFournisseur->revertAll();
        return;
    }

    modelFournisseur->select(); // Rafraîchir le tableau du formulaire principal
    QMessageBox::information(this, "Succès", "Fournisseur ajouté avec succès !");

    // Vider les champs après ajout
    ui->lineEditRefFrs->clear();
    ui->lineEditRaisonSociale->clear();
    ui->lineEditNIF->clear();
    ui->lineEditAdresse->clear();
    ui->lineEditTelFrs->clear();
}
