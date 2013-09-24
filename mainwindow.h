#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class LargeTableModel;
class LargeTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    std::shared_ptr<LargeTableModel> ltm;
    std::shared_ptr<LargeTableView> ltv;

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void newMessage();
};

#endif // MAINWINDOW_H
