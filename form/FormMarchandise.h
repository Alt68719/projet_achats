#ifndef FORMMARCHANDISE_H
#define FORMMARCHANDISE_H

#endif // FORMMARCHANDISE_H

#pragma once
#include <QDialog>

namespace Ui { class FormMarchandise; }

class FormMarchandise : public QDialog
{
    Q_OBJECT

public:
    explicit FormMarchandise(QWidget *parent = nullptr);
    ~FormMarchandise();

private slots:
    void on_btnAjouter_clicked();

private:
    Ui::FormMarchandise *ui;
};

