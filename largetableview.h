#ifndef LARGETABLEVIEW_H
#define LARGETABLEVIEW_H

#include <QAbstractScrollArea>
#include <QWheelEvent>
#include <memory>

class LargeTable;
class LargeTableModel;

class LargeTableView : public QAbstractScrollArea
{
    Q_OBJECT

    std::shared_ptr<LargeTableModel> ltm;
    std::shared_ptr<LargeTable> lt;

public:
    explicit LargeTableView(std::shared_ptr<LargeTableModel> &ltm, QWidget *parent = 0);

protected:
    virtual void wheelEvent(QWheelEvent *);

private slots:
    void heightsChanged();
    void scrolled(QWheelEvent *);
    void scrolled(int);
};

#endif // LARGETABLEVIEW_H
