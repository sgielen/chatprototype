#include "largetablemodel.h"

LargeTableModel::LargeTableModel(QObject *parent)
: QObject(parent)
{
}

LargeTableModel::RowIndex LargeTableModel::push_back(const Row &r) {
    RowIndex index = rows_.push_back(r);
    emit newRowAdded(index);
    return index;
}


LargeTableModel::RowIndex LargeTableModel::push_front(const Row &r) {
    RowIndex index = rows_.push_front(r);
    emit newRowAdded(index);
    return index;
}

const LargeTableModel::Row &LargeTableModel::at(RowIndex index) const {
    return rows_.at(index);
}

LargeTableModel::Row &LargeTableModel::at(RowIndex index) {
    return rows_.at(index);
}

const index_container<LargeTableModel::Row> &LargeTableModel::rows() const {
    return rows_;
}

const std::deque<LargeTableModel::RowIndex> &LargeTableModel::indices() const {
    return rows_.indices();
}
