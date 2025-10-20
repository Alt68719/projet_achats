#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSqlTableModel"

// Formulaires spécifiques
#include "form/FormFournisseur.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Créer le modèle
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("FOURNISSEUR"); // Nom de votre table

    // 2. Définir les noms des colonnes pour l'affichage
    model->setHeaderData(0, Qt::Horizontal, tr("Référence Frs"));
    model->setHeaderData(1, Qt::Horizontal, tr("Raison Sociale"));
    model->setHeaderData(2, Qt::Horizontal, tr("NIF"));
    model->setHeaderData(3, Qt::Horizontal, tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, tr("Téléphone"));

    // 3. Charger les données
    if (!model->select()) {
        QMessageBox::critical(this, "Erreur SQL", "Impossible de charger la table FOURNISSEUR.");
        return;
    }

    // 4. Lier le modèle à la QTableView (assurez-vous d'avoir une QTableView nommée 'tableView' dans votre .ui)
    //ui->tableView->setModel(model);
    //ui->tableView->setEditTriggers(QAbstractItemView::DoubleTap | QAbstractItemView::EditKeyPressed);
    // Permet l'édition directe dans la vue, simplifiant l'UPDATE

    // Connexion des boutons si non déjà fait dans Qt Designer
    connect(ui->btnNouveauAchat, &QPushButton::clicked, this, &MainWindow::on_btnNouveauAchat_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// -------- Bouton Nouveau Achat --------
//dep
void MainWindow::on_action_fournisseurs_triggered()
{
    // 'this' est le parent de la nouvelle fenêtre.
    // Si c'est une QDialog, show() la rend non modale, exec() la rend modale.
    //FormFournisseur fournisseurDialog(this);
    //fournisseurDialog.exec(); // Ouvre la fenêtre et bloque l'interaction avec le parent
}
//fin
void MainWindow::on_btnNouveauAchat_clicked()
{

    FormFournisseur fFournisseur(this);
    if(fFournisseur.exec() == QDialog::Rejected) return;

    FormMarchandise fMarchandise(this);
    if(fMarchandise.exec() == QDialog::Rejected) return;

    FormEntrepot fEntrepot(this);
    if(fEntrepot.exec() == QDialog::Rejected) return;

    FormLivraison fLivraison(this);
    if(fLivraison.exec() == QDialog::Rejected) return;

    FormBonLivraison fBon(this);
    fBon.exec();  // Affiche le bouton générer le bon
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
    Entrepot fEntrepot(this);  // <-- utiliser la classe Entrepot correspondant à Entrepot.ui
    fEntrepot.exec();           // Affiche la fenêtre en modal
}
