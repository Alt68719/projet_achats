#ifndef FORMBONLIVRAISON_H
#define FORMBONLIVRAISON_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class FormBonLivraison;
}

class FormBonLivraison : public QDialog
{
    Q_OBJECT

public:
    explicit FormBonLivraison(QWidget *parent = nullptr);
    ~FormBonLivraison();

private:
    Ui::FormBonLivraison *ui;
    QSqlQueryModel *modelBonLivraison;

    void chargerBonLivraison(); // nouvelle fonction
};

#endif // FORMBONLIVRAISON_H
