#ifndef FORMBONLIVRAISON_H
#define FORMBONLIVRAISON_H

#include <QDialog>

namespace Ui {
class FormBonLivraison;
}

class FormBonLivraison : public QDialog
{
    Q_OBJECT

public:
    explicit FormBonLivraison(QWidget *parent = nullptr);
    ~FormBonLivraison();

private slots:
    void on_btnGenerer_clicked(); // Slot pour le bouton "Générer le bon de livraison"

private:
    Ui::FormBonLivraison *ui;
};

#endif // FORMBONLIVRAISON_H
