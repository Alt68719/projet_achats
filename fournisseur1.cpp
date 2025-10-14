#include "fournisseur1.h"

fournisseur1::fournisseur1(QObject *parent)
    : QAbstractItemModel(parent)
{}

QVariant fournisseur1::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex fournisseur1::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex fournisseur1::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int fournisseur1::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int fournisseur1::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant fournisseur1::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
