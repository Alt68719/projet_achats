#ifndef FORMENTREPOT_H
#define FORMENTREPOT_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class FormEntrepot;
}

class FormEntrepot : public QDialog
{
    Q_OBJECT

public:
    explicit FormEntrepot(QWidget *parent = nullptr);
    ~FormEntrepot();

private slots:
    void on_btnAjouter_clicked();

private:
    Ui::FormEntrepot *ui;
    QSqlQueryModel *modelEntrepot;

    void remplirComboBoxes();
};

#endif // FORMENTREPOT_H
