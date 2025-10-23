#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSqlTableModel"

#include "database/database.h"

#include "form/FormAjouterFournisseur.h"
#include "form/FormListeFournisseur.h"
#include "form/FormMarchandise.h"
#include "form/FormEntrepot.h"
#include "form/FormLivraison.h"
#include "form/FormBonLivraison.h"
#include "form/ViewStock.h"
#include "form/ViewInventaire.h"
#include "form/Entrepot.h"
//#include "form/ViewListeLivraison.h"
//#include "form/ViewAPropos.h"

#include <QMessageBox>



// ... (Autres includes pour Entrepot, Marchandise, etc.)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Assurez-vous d'ouvrir la connexion à la base de données ici, par exemple :
    // Database::ouvrir();

}

MainWindow::~MainWindow()
{
    delete ui;
    // Database::fermer();
}

void MainWindow::on_btnGestionFournisseurs_clicked()
{
    FormListeFournisseur *form = new FormListeFournisseur(this);
    form->exec();
}
void MainWindow::on_action_fournisseurs_triggered()
{
    // 'this' est le parent de la nouvelle fenêtre.
    // Si c'est une QDialog, show() la rend non modale, exec() la rend modale.
    //FormFournisseur fournisseurDialog(this);
    //fournisseurDialog.exec(); // Ouvre la fenêtre et bloque l'interaction avec le parent
}

// Vous feriez de même pour Marchandise, Stock, etc.
void MainWindow::on_btnNouveauAchat_clicked()
{
    QSqlTableModel modelFournisseur(nullptr, Database::db);
    modelFournisseur.setTable("fournisseur");
    modelFournisseur.select();

    FormAjouterFournisseur fAjouterFournisseur(&modelFournisseur, this);
    fAjouterFournisseur.exec();
}


// -------- Autres boutons --------
void MainWindow::on_btnVoirStock_clicked()
{
    ViewStock view(this);
    view.exec();
}

void MainWindow::on_btnInventaire_clicked()
{
    ViewInventaire view(this);
    view.exec();
}

void MainWindow::on_btnListeLivraison_clicked()
{
    FormBonLivraison view(this);
    view.exec();
}

void MainWindow::on_btnAPropos_clicked()
{
    FormMarchandise view(this);
    view.exec();
}

void MainWindow::on_btnVoirEntrepot_clicked()
{
    Entrepot fEntrepot(this);
    fEntrepot.exec();
}
void MainWindow::on_btnVoirFourniseur_clicked()
{
    FormListeFournisseur fFourniseur(this);
    fFourniseur.exec();
}
