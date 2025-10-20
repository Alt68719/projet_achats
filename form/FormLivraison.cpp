#include "FormLivraison.h"
#include "ui_FormLivraison.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDate> // Assurez-vous que QDate est inclus

FormLivraison::FormLivraison(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLivraison)
{
    ui->setupUi(this);
    // Définir la date par défaut sur la date d'aujourd'hui
    ui->dateEdit->setDate(QDate::currentDate());
}

FormLivraison::~FormLivraison()
{
    delete ui;
}

/**
 * @brief Met à jour la quantité de stock pour une marchandise dans un entrepôt donné.
 * Si l'article existe déjà, la quantité est ajoutée. Sinon, une nouvelle entrée est créée.
 * @param ref_mar Référence de la marchandise (num_mar).
 * @param ref_entrepot Référence de l'entrepôt (ref_entr).
 * @param qte_livree Quantité à ajouter au stock.
 */
void FormLivraison::mettreAJourStock(const QString &ref_mar, const QString &ref_entrepot, int qte_livree)
{
    // 1. Vérifier si l'article existe déjà dans le stock (table 'stocker')
    QSqlQuery selectQuery(Database::db);
    selectQuery.prepare("SELECT qte_stock FROM stocker WHERE num_mar = ? AND ref_entr = ?");
    selectQuery.addBindValue(ref_mar);
    selectQuery.addBindValue(ref_entrepot);

    if (selectQuery.exec() && selectQuery.next()) {
        // L'article existe : Mettre à jour la quantité
        int qteExistante = selectQuery.value(0).toInt();
        int qteNouvelle = qteExistante + qte_livree;

        QSqlQuery updateQuery(Database::db);
        // ATTENTION: J'ai utilisé 'num_mar' dans la clause WHERE pour être cohérent avec le SELECT et le INSERT ci-dessous.
        // Assurez-vous que 'ref_mar' que vous passez correspond bien à la colonne 'num_mar' de votre table 'stocker'.
        updateQuery.prepare("UPDATE stocker SET qte_stock = ? WHERE num_mar = ? AND ref_entr = ?");
        updateQuery.addBindValue(qteNouvelle);
        updateQuery.addBindValue(ref_mar);
        updateQuery.addBindValue(ref_entrepot);

        if (!updateQuery.exec()) {
            qWarning() << "Erreur lors de la mise à jour du stock:" << updateQuery.lastError().text();
            QMessageBox::warning(this, "Erreur Stock", "Erreur lors de la mise à jour du stock existant.");
        }

    } else {
        // L'article n'existe pas : Insérer une nouvelle ligne
        QSqlQuery insertQuery(Database::db);
        insertQuery.prepare("INSERT INTO stocker(num_mar, ref_entr, qte_stock) VALUES (?, ?, ?)");
        insertQuery.addBindValue(ref_mar);
        insertQuery.addBindValue(ref_entrepot);
        insertQuery.addBindValue(qte_livree);

        if (!insertQuery.exec()) {
            qWarning() << "Erreur lors de l'insertion du stock:" << insertQuery.lastError().text();
            QMessageBox::warning(this, "Erreur Stock", "Erreur lors de l'insertion d'un nouveau stock.");
        }
    }
}

void FormLivraison::on_btnAjouter_clicked()
{
    // 1. Récupération des données du formulaire
    QString num_livraison = ui->lineNumLivraison->text();
    QString ref_frs = ui->lineRefFournisseur->text();
    QDate date_de_livraison = ui->dateEdit->date();
    // Utiliser `text()` pour le prix, le convertir en double
    double prix_achat = ui->linePrix->text().toDouble();
    // Utiliser `text()` pour la quantité, le convertir en int
    int qte_livre = ui->lineQte->text().toInt();
    QString ref_mar = ui->lineRefMar->text();
    QString ref_entrepot = ui->lineRefEntr->text();

    // Vérification rapide des entrées cruciales
    if (num_livraison.isEmpty() || ref_mar.isEmpty() || ref_entrepot.isEmpty() || qte_livre <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez remplir tous les champs obligatoires (Numéro, Références, Quantité > 0).");
        return;
    }

    // 2. Insertion de la livraison dans la table 'livrer'
    QSqlQuery query(Database::db);
    // Assurez-vous que l'ordre des colonnes correspond à l'ordre des addBindValue
    query.prepare("INSERT INTO livrer(num_livraison, ref_frs, date_de_livraison, prix_achat, qte_livre, ref_mar, ref_entrepot) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(num_livraison);
    query.addBindValue(ref_frs);
    query.addBindValue(date_de_livraison);
    query.addBindValue(prix_achat);
    query.addBindValue(qte_livre);
    query.addBindValue(ref_mar);
    query.addBindValue(ref_entrepot);

    if (!query.exec()){
        QMessageBox::warning(this,"Erreur d'Insertion Livraison","Impossible d'ajouter la livraison:\n"+query.lastError().text());
        qWarning() << "Erreur SQL (Insertion Livraison):" << query.lastError().text();
        return;
    }

    // 3. Mise à jour du stock dans la table 'stocker'
    mettreAJourStock(ref_mar, ref_entrepot, qte_livre);

    // 4. Succès et fermeture
    QMessageBox::information(this, "Succès", "La livraison a été ajoutée et le stock mis à jour.");
    this->accept();
}
