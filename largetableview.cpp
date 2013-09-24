#include "largetableview.h"
#include <QVBoxLayout>
#include <QScrollBar>
#include "largetable.h"

LargeTableView::LargeTableView(std::shared_ptr<LargeTableModel> &ltm, QWidget *parent)
: QAbstractScrollArea(parent)
, ltm(ltm)
, lt(new LargeTable(ltm))
{
    connect(lt.get(), SIGNAL(heightsChanged()), SLOT(heightsChanged()));
    connect(lt.get(), SIGNAL(scrolled(QWheelEvent*)), SLOT(scrolled(QWheelEvent*)));
    connect(verticalScrollBar(), SIGNAL(sliderMoved(int)), SLOT(scrolled(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), lt.get(), SLOT(scrollToHeight(int)));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lt.get());
    mainLayout->setMargin(0);
    setLayout(mainLayout);
}

void LargeTableView::scrolled(QWheelEvent *event) {
    QPoint pixelDelta;
    QPoint angleDelta;
#if QT_VERSION >= 0x050000
    pixelDelta = event->pixelDelta();
    angleDelta = event->angleDelta();
#else
    angleDelta = QPoint(0, event->delta() / 3);
#endif

    int yPixelDelta;
    if(!pixelDelta.isNull()) {
        yPixelDelta = pixelDelta.y();
    } else {
        yPixelDelta = angleDelta.y();
    }

    scrolled(yPixelDelta);
}

void LargeTableView::scrolled(int yPixelDelta) {
    QScrollBar *sb = verticalScrollBar();
    int scrollBarValue = sb->value() - yPixelDelta;
    if(scrollBarValue < 0) {
        scrollBarValue = 0;
    } else if(scrollBarValue > sb->maximum()) {
        scrollBarValue = sb->maximum();
    }
    sb->setValue(scrollBarValue);
}

void LargeTableView::wheelEvent(QWheelEvent *event) {
    scrolled(event);
    event->accept();
}

void LargeTableView::heightsChanged() {
    int emulatedHeight = lt->getEmulatedHeight();
    QScrollBar *sb = verticalScrollBar();
    bool atBottom = sb->maximum() == sb->value();
    bool screenNotFilled = emulatedHeight < height();
    if(screenNotFilled) {
        sb->setVisible(false);
        lt->scrollToHeight(0);
    } else {
        sb->blockSignals(true);
        sb->setMinimum(0);
        sb->setMaximum(emulatedHeight - height());
        if(atBottom) {
            sb->setValue(emulatedHeight - height());
        }
        sb->blockSignals(false);
        lt->scrollToHeight(sb->value());
        sb->setVisible(true);
    }
}
