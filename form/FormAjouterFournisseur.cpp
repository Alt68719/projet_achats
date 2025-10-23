#include "FormAjouterFournisseur.h"
#include "ui_FormAjouterFournisseur.h"

#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>

// Inclusion des formulaires suivants
#include "form/FormMarchandise.h"
#include "form/FormEntrepot.h"
#include "form/FormLivraison.h"

FormAjouterFournisseur::FormAjouterFournisseur(QSqlTableModel *modelFrs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormAjouterFournisseur),
    modelFournisseur(modelFrs)
{
    ui->setupUi(this);
}

FormAjouterFournisseur::~FormAjouterFournisseur()
{
    delete ui;
}

void FormAjouterFournisseur::on_btnAjouter_clicked()
{
    QString ref = ui->lineEditRefFrs->text().trimmed();
    QString raisonSociale = ui->lineEditRaisonSociale->text().trimmed();

    bool nifOk, telOk;
    int NIF = ui->lineEditNIF->text().trimmed().toInt(&nifOk);
    int tel = ui->lineEditTelFrs->text().trimmed().toInt(&telOk);
    QString adresse = ui->lineEditAdresse->text().trimmed();

    if (ref.isEmpty() || raisonSociale.isEmpty() || adresse.isEmpty() || !nifOk || !telOk) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Veuillez remplir tous les champs correctement (NIF et Tél doivent être des nombres).");
        return;
    }

    QSqlRecord newRecord = modelFournisseur->record();
    newRecord.setValue("ref_frs", ref);
    newRecord.setValue("raison_sociale", raisonSociale);
    newRecord.setValue("NIF", NIF);
    newRecord.setValue("adresse", adresse);
    newRecord.setValue("tel_frs", tel);

    if (!modelFournisseur->insertRecord(-1, newRecord)) {
        QMessageBox::critical(this, "Erreur SQL (Insertion)",
                              "Impossible d'ajouter le fournisseur : " + modelFournisseur->lastError().text());
        modelFournisseur->revertAll();
        return;
    }

    if (!modelFournisseur->submitAll()) {
        QMessageBox::critical(this, "Erreur SQL (Sauvegarde)",
                              "Échec de la sauvegarde en base : " + modelFournisseur->lastError().text());
        modelFournisseur->revertAll();
        return;
    }

    modelFournisseur->select();
    QMessageBox::information(this, "Succès", "Fournisseur ajouté avec succès !");

    // ✅ Ferme ce formulaire
    this->accept();

    // 🔁 Enchaînement automatique des formulaires
    QWidget *parentWidget = this->parentWidget();

    // --- Étape 1 : Marchandise ---
    FormMarchandise formMarchandise(parentWidget);
    if (formMarchandise.exec() == QDialog::Rejected)
        return;

    // --- Étape 2 : Entrepôt ---
    FormEntrepot formEntrepot(parentWidget);
    if (formEntrepot.exec() == QDialog::Rejected)
        return;

    // --- Étape 3 : Livraison ---
    FormLivraison formLivraison(parentWidget);
    formLivraison.exec();

    // ✅ Retour automatique à MainWindow (fin du flux)
}
