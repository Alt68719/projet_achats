#ifndef FORMMARCHANDISE_H
#define FORMMARCHANDISE_H



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
    void slotAjouterMarchandise();

private:
    Ui::FormMarchandise *ui;
};
#endif // FORMMARCHANDISE_H

