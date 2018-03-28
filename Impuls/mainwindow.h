#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"
#include "ui_mainwindow.h"
#include "pozycjedokumentowgateway.h"

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
    Connection* con = Connection::getConnection();

    Ui::MainWindow *ui;
    QSqlQueryModel *modelDok;
    QSqlQueryModel *modelPoz;

    PozycjeDokumentowGateway pozycjeDokumentow;

    QString sQuery;

    void resizeColumns(QTableView* table);
    void resizeEvent(QResizeEvent *evt);
    int selectedDokID();

};

#endif // MAINWINDOW_H
