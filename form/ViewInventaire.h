#ifndef VIEWINVENTAIRE_H
#define VIEWINVENTAIRE_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class ViewInventaire;
}

class ViewInventaire : public QDialog
{
    Q_OBJECT

public:
    explicit ViewInventaire(QWidget *parent = nullptr);
    ~ViewInventaire();

private slots:
    void on_btnAfficher_clicked();

private:
    Ui::ViewInventaire *ui;
    QSqlQueryModel *model;
};

#endif // VIEWINVENTAIRE_H
