#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_clicked();

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *modelDok;
    QSqlQueryModel *modelPoz;

    QString sQuery;

    void resizeColumns(QTableView* table);
    void resizeEvent(QResizeEvent *evt);

    Connection* con = Connection::getConnection();

};

#endif // MAINWINDOW_H
