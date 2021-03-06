#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->modelDok = new QSqlQueryModel();
    char * queryLiteralDok = R"(
                          select
                          d.ID_DOK
                          ,d.KOD_DOKUMENTU
                          ,d.NUMER_DOKUMENTU
                          ,d.DATA_WYSTAWIENIA
                          ,d.DATA_REALIZACJI
                          ,m.SYMBOL_MAGAZYNU
                          ,k.NAZWA_SKROCONA
                          ,d.WPROWADZAJACY
                          ,d.ZATWIERDZAJACY
                          from dokumenty d, magazyny m, kontrahenci k
                          where d.RODZAJ_DOKUMENTU = 'PZ'
                          and d.MAGAZYN = m.MAGAZYN
                          and d.NUMER_KONTRAHENTA=k.NUMER_KONTRAHENTA
                          order by 5 desc
            )";



    modelDok->setQuery(queryLiteralDok);
    ui->tableView->setModel(modelDok);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    this->modelPoz = new QSqlQueryModel();
    ui->tableView_2->setModel(modelPoz);


    QObject::connect(ui->tableView, SIGNAL(clicked(QModelIndex)),
                     this,SLOT(on_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clicked(){
    volatile int dokID = selectedDokID();
    modelPoz->setQuery(*pozycjeDokumentow.select(dokID));
    ui->tableView_2->setModel(modelPoz);
}

void MainWindow::resizeColumns(QTableView* table ){
    int columnsCount = table->model()->columnCount();
    for (int i = columnsCount; i>0; i--){
        table->setColumnWidth(i-1, this->width()/columnsCount);
    }
}

void MainWindow::resizeEvent(QResizeEvent *evt){
    resizeColumns(ui->tableView);
    QMainWindow::resizeEvent(evt); //call base implementation
}

int MainWindow::selectedDokID(){
    int rowNum = ui->tableView->currentIndex().row();
    int colNum = 0;
    QModelIndex index = ui->tableView->model()->index(rowNum, colNum, QModelIndex());

    return ui->tableView->model()->data(index).toInt();
}

