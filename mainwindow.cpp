#include "mainwindow.h"
#include <QTimer>
#include <cmath>
#include <cstdio>
#include <sstream>
#include "largetablemodel.h"
#include "largetableview.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ltm(new LargeTableModel())
, ltv(new LargeTableView(ltm))
{
    setCentralWidget(ltv.get());
    resize(800, 600);

    QTimer::singleShot(1000, this, SLOT(newMessage()));
}

void MainWindow::newMessage() {
    typedef LargeTableModel::Row Row;
    static time_t t_ = 0;
    int iterations = rand() % 4 + 5;
    for(int i = 0; i < iterations; ++i) {
        t_ += 10 + rand() % 90;
        time_t t = t_;
        int hr, min, sec;
        hr = std::floor(t / 3600);
        t = t % 3600;
        min = std::floor(t / 60);
        t = t % 60;
        sec = t;
        char *time;
        asprintf(&time, "[%02d:%02d:%02d]", hr, min, sec);
        std::stringstream message;
        message << "This is a test message with time " << t_;
        Row row = {time, "<Nickname>", message.str()};
        free(time);
        ltm->push_back(row);
    }
    QTimer::singleShot(1500, this, SLOT(newMessage()));
}
