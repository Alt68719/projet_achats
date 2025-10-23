#ifndef FORMAJOUTERFOURNISSEUR_H
#define FORMAJOUTERFOURNISSEUR_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class FormAjouterFournisseur;
}

class FormAjouterFournisseur : public QDialog
{
    Q_OBJECT

public:
    //explicit FormAjouterFournisseur(QWidget *parent = nullptr);
    // Le constructeur prend le modèle de la liste en référence pour pouvoir y insérer des données
    explicit FormAjouterFournisseur(QSqlTableModel *modelFrs, QWidget *parent = nullptr);
    ~FormAjouterFournisseur();

private slots:
    void on_btnAjouter_clicked();

private:
    Ui::FormAjouterFournisseur *ui;
    QSqlTableModel *modelFournisseur; // Pointeur vers le modèle du formulaire principal
};

#endif // FORMAJOUTERFOURNISSEUR_H
