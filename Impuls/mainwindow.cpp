#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->modelDok = new QSqlQueryModel();
    char * queryLiteral = R"(
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
            )";



    modelDok->setQuery(queryLiteral);
    ui->tableView->setModel(modelDok);

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

    int rowNum = ui->tableView->currentIndex().row();
    int colNum = 0;
    QModelIndex index = ui->tableView->model()->index(rowNum, colNum, QModelIndex());
    int dokID = ui->tableView->model()->data(index).toInt();

    QSqlQuery qry;
    qry.prepare("select * from pozycje_dokumentow pd where pd.ID_DOK = :dok");
    qry.bindValue(":dok",dokID);
    qry.exec();
    modelPoz->setQuery(qry);
    ui->tableView_2->setModel(modelPoz);
}
