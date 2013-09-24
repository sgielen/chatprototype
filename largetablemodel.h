#ifndef LARGETABLEMODEL_H
#define LARGETABLEMODEL_H

#include <QObject>
#include <string>
#include <deque>
#include "astrant-extensions/index_container.hpp"

class LargeTableModel : public QObject
{
    Q_OBJECT

public:
    typedef std::string Column;
    typedef std::vector<Column> Row;
    typedef index_container<Row>::size_type RowIndex;

private:
    index_container<Row> rows_;

public:
    explicit LargeTableModel(QObject *parent = 0);
    RowIndex push_back(const Row &r);
    RowIndex push_front(const Row &r);
    const Row &at(RowIndex) const;
    Row &at(RowIndex);
    const index_container<Row> &rows() const;
    const std::deque<RowIndex> &indices() const;

signals:
    void newRowAdded(LargeTableModel::RowIndex);
};

#endif // LARGETABLEMODEL_H
