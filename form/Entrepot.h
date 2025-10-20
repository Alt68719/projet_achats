#ifndef ENTREPOT_H
#define ENTREPOT_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class Entrepot;
}

class Entrepot : public QDialog
{
    Q_OBJECT

public:
    explicit Entrepot(QWidget *parent = nullptr);
    ~Entrepot();

private slots:
    void on_btnAjouter_clicked();
    void on_btnSupprimer_clicked();

private:
    Ui::Entrepot *ui;
    QSqlTableModel *model;
};

#endif // ENTREPOT_H
