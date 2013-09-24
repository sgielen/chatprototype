#include "largetable.h"
#include <cassert>
#include <QLabel>
#include <QWheelEvent>
#include <QVBoxLayout>

LargeTable::LargeTable(std::shared_ptr<LargeTableModel> ltm, QWidget *parent)
: QWidget(parent)
, ltm(ltm)
{
    connect(ltm.get(), SIGNAL(newRowAdded(LargeTableModel::RowIndex)), SLOT(newRowAdded(LargeTableModel::RowIndex)));
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

unsigned int LargeTable::getHeightForRow(const LargeTableModel::Row &r) const {
    // This depends on the number of columns and their widths. For each
    // column, make a QLabel (or whatever is used for drawing), give it
    // the width of the column, fill it with the text from the row as
    // normal, and compute its height. Then return the maximum height seen
    // while iterating through the columns.
    assert(columnWidths.size() == r.size());
    // For now, assume that QLabel word wraps every cell.
    unsigned int maxHeight = 0;
    for(unsigned int i = 0; i < r.size(); ++i) {
        unsigned int width = columnWidths[i];
        std::string content = r[i];
        QLabel label(QString::fromStdString(content));
        label.setWordWrap(true);
        unsigned int height = label.heightForWidth(width);
        if(maxHeight < height) {
            maxHeight = height;
        }
    }
    return maxHeight;
}

unsigned int LargeTable::getEmulatedHeight() const {
    unsigned int height = 0;
    for(auto &it : heights) {
        height += it.second;
    }
    return height;
}

void LargeTable::newRowAdded(LargeTableModel::RowIndex ri) {
    assert(sender() == ltm.get());
    const LargeTableModel::Row &r = ltm->at(ri);
    assert(r.size() > 0);
    heights[ri] = getHeightForRow(ltm->at(ri));
    emit heightsChanged();
}

std::pair<std::list<LargeTableModel::RowIndex>, unsigned int> LargeTable::rowsAndOffsetForHeight(unsigned int wantedHeight) const {
    unsigned int height = 0;
    if(ltm->indices().empty()) {
        return std::make_pair(std::list<LargeTableModel::RowIndex>(), 0);
    }

    const std::deque<LargeTableModel::RowIndex> &indices = ltm->indices();
    std::list<LargeTableModel::RowIndex> list;
    unsigned int offset;
    unsigned int fullScreenSize = wantedHeight + this->height();
    for(const LargeTableModel::RowIndex &it : indices) {
        if(!list.empty()) {
            // First message to display already found, append more...
            list.push_back(it);
            // ...until we have enough
            if(height + heights.at(it) >= fullScreenSize) {
                return std::make_pair(list, offset);
            }
        } else if(height + heights.at(it) > wantedHeight) {
            // First message to display found, start appending messages
            list.push_back(it);
            offset = wantedHeight - height;
        }
        height += heights.at(it);
    }

    // If we came here, it means we found messages to display, but not enough to fill
    // the screen. This should not be possible unless there are actually no more messages
    // to display (the scroll position cannot go beyond the last screen of messages).
    assert(list.size() == indices.size());

    return std::make_pair(list, offset);
}

void LargeTable::scrollToHeight(int height) {
    auto rowsAndOffset = rowsAndOffsetForHeight(height);
    std::list<LargeTableModel::RowIndex> rows = rowsAndOffset.first;
    int offset = -rowsAndOffset.second;
    // The messages indicated by <rows> should be drawn from the top of the screen
    // Its top position should be <offst> pixels above the top of the screen
    // clear it
    QList<QWidget*> widgets = findChildren<QWidget*>();
    foreach(QWidget *widget, widgets) {
        delete widget;
    }
    for(LargeTableModel::RowIndex ri : rows) {
        unsigned int width = 0;
        LargeTableModel::Row &r = ltm->at(ri);
        for(unsigned int column = 0; column < r.size(); ++column) {
            QLabel *label = new QLabel(QString::fromStdString(r.at(column)), this);
            label->resize(columnWidths[column], heights[ri]);
            label->setAlignment(Qt::AlignTop);
            label->move(width, offset);
            label->show();
            label->setWordWrap(true);
            label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
            width += columnWidths[column];
        }
        offset += heights[ri];
    }
}

void LargeTable::updateAllHeights() {
    for(const LargeTableModel::RowIndex &it : ltm->indices()) {
        heights[it] = getHeightForRow(ltm->at(it));
    }
    emit heightsChanged();
}

void LargeTable::resizeEvent(QResizeEvent *) {
    // TODO make these configurable
    columnWidths = {100, 100, width() - 200};
    updateAllHeights();
}

void LargeTable::wheelEvent(QWheelEvent *event) {
    // Ignoring the event does not seem to help in delivering it to the
    // LargeTableView, so we deliver it ourselves
    emit scrolled(event);
    event->accept();
}
