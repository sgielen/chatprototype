#ifndef LARGETABLE_H
#define LARGETABLE_H

#include <QWidget>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include "largetablemodel.h"

class LargeTable : public QWidget, public std::enable_shared_from_this<LargeTable>
{
    Q_OBJECT

    std::shared_ptr<LargeTableModel> ltm;
    std::map<LargeTableModel::RowIndex, unsigned int> heights;
    std::vector<unsigned int> columnWidths;

public:
    explicit LargeTable(std::shared_ptr<LargeTableModel> ltm, QWidget *parent = 0);
    unsigned int getHeightForRow(const LargeTableModel::Row &r) const;
    unsigned int getEmulatedHeight() const;
    void updateAllHeights();

    //const std::vector<unsigned int> &getColumnWidths() { return columnWidths; }
    //void setColumnWidths(const std::vector<unsigned int> &columnWidths) { this->columnWidths = columnWidths; }

public slots:
    void scrollToHeight(int);

signals:
    void heightsChanged();
    void scrolled(QWheelEvent *event);

private:
    std::pair<std::list<LargeTableModel::RowIndex>,unsigned int> rowsAndOffsetForHeight(unsigned int) const;

private slots:
    void newRowAdded(LargeTableModel::RowIndex ri);
    virtual void resizeEvent(QResizeEvent *);
    virtual void wheelEvent(QWheelEvent *);
};

#endif // LARGETABLE_H
