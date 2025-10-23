#ifndef FORMLISTEFOURNISSEUR_H
#define FORMLISTEFOURNISSEUR_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class FormListeFournisseur;
}

class FormListeFournisseur : public QDialog
{
    Q_OBJECT

public:
    explicit FormListeFournisseur(QWidget *parent = nullptr);
    ~FormListeFournisseur();

private slots:
    // Ouvre le formulaire d'ajout
    void on_btnOuvrirAjout_clicked();

    // Opération DELETE : Supprimer l'enregistrement sélectionné
    void on_btnSupprimer_clicked();

    // Opération UPDATE : Enregistrer les modifications
    void on_btnEnregistrerModifications_clicked();

private:
    Ui::FormListeFournisseur *ui;
    QSqlTableModel *model;

    void configurerModele();
    void configurerTableau();
};

#endif // FORMLISTEFOURNISSEUR_H
