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

    int rowNum = ui->tableView->currentIndex().row();
    int colNum = 0;
    QModelIndex index = ui->tableView->model()->index(rowNum, colNum, QModelIndex());
    volatile int dokID = ui->tableView->model()->data(index).toInt();

    modelPoz->setQuery(getPozycjeDokumentu(dokID,1));
    ui->tableView_2->setModel(modelPoz);
}

void MainWindow::resizeColumns(QTableView* table ){
    int columnsCount = table->model()->columnCount();
    for (int i = columnsCount; i>0; i--){
        table->setColumnWidth(i-1, this->width()/columnsCount);
    }
}

void MainWindow::resizeEvent(QResizeEvent *evt)
{
    resizeColumns(ui->tableView);
    QMainWindow::resizeEvent(evt); //call base implementation
}

QSqlQuery MainWindow::getPozycjeDokumentu(int id_dok, int id_firma){
    char * queryLiteralDokPoz = R"(
                                SELECT   pd.ROWID, pd.archiwum_serii, pd.cena, pd.cena_domyslna,
                                         pd.cena_jednostkowa_brutto, pd.cena_jednostkowa_netto,
                                         pd.cena_jednostkowa_netto_dewiz, pd.cena_sprzedazy,
                                         pd.cena_sprzedazy_dewiz, pd.cena_w_walucie, pd.cena_z_zamowienia,
                                         pd.clo, pd.czy_cena_na_fs, pd.czy_cena_na_fs_w_walucie,
                                         pd.czy_ceny_alternatywne, pd.czy_zbyt, pd.data_modyfikacji,
                                         pd.data_waznosci, pd.data_wprowadzenia, pd.docelowe_mcs, pd.i_,
                                         pd.id_archiwum_serii, pd.id_dok, pd.id_eksportu,
                                         pd.id_kontroli_jakosci, pd.ile_pozostalo, pd.ile_pozostalo_bo,
                                         pd.ilosc, pd.ilosc_altjm, pd.ilosc_jm_sprzedazy, pd.ilosc_wymagana,
                                         pd.ilosc_wymagana_altjm, pd.indeks_czesci, pd.jm_ceny_sprzedazy,
                                         pd.jm_kod_jednostka_miary, pd.jm_sprzedazy, pd.kod_konta_m,
                                         pd.kod_stawki_vat, pd.kod_waluty_sprzedazy, pd.koryguj_dok,
                                         pd.koryguj_poz, pd.kurs_waluty_sprzedazy, pd.kwota_rabatu,
                                         pd.magazyn, pd.magazynowa_jm, pd.miejsce_skladowania,
                                         pd.modyfikujacy, pd.nr_serii, pd.numer_cennika,
                                         pd.numer_cennika_zakupu, pd.numer_paczki, pd.lp,
                                         pd.numer_pozycji_przydzialu, pd.numer_zamowienia,
                                         pd.numer_zamowienia_zw, pd.numer_zamowienia_zz, pd.numer_zlecenia,
                                         pd.numer_zlecenia_docelowego, pd.numer_zlecenia_z_dokumentu,
                                         pd.pm_cykl_produkcyjny, pd.pm_cykl_produkcyjny1, pd.pm_id_receptury,
                                         pd.pm_kod_operacji, pd.powiazania_kaucji_opakowan, pd.powiazania_mm,
                                         pd.powiazania_opakowan, pd.powiazania_pdm_hd, pd.powiazania_pdm_pfs,
                                         pd.powiazania_wydan_opakowan, pd.pozycja_dokumentu,
                                         pd.pozycja_kalkulacji_kosztow, pd.pozycja_zamowienia,
                                         pd.pozycja_zamowienia_zw, pd.pozycja_zamowienia_zz,
                                         pd.procent_rabatu, pd.przelicznik_jm, pd.przelicznik_jm_sprzedazy,
                                         pd.rabat_kwota_dewiz, pd.rabat_procent_dewiz, pd.rt_id_firmy,
                                         pd.rt_numer_rezerwacji, pd.rt_numer_roku, pd.seria_czesci_nadrzednej,
                                         pd.stanowisko_kosztow, pd.status, pd.stawka_vat,
                                         pd.tryb_rabatow_upustow, pd.upust_procent, pd.uwagi,
                                         pd.uzupelniac_niedobory, pd.w_, pd.wartosc_edytowana,
                                         pd.wprowadzajacy, pd.wycenil_dok, pd.wycenil_poz, pd.zapas#,
                                         pd.znak_dok, pd.cena_po_przeszacowaniu,
                                         NVL(pd.id_pzk, pzk.id_pzk) intcalc_zk, pd.id_pzk,
                                         NVL(pd.id_pzz, pzz.id_pzz) intcalc_zz, pd.id_pzz,
                                         NVL(pd.id_rezerwacji, rt.id_rezerwacji) intcalc_rez, pd.id_rezerwacji,
                                         i.indeks, i.nazwa_czesci,
                                         nvl(pd.cena_sprzedazy * pd.ilosc_jm_sprzedazy, 0) wartosc_pozycji_sprz,
                                         CASE WHEN ROWNUM < 0 THEN LPAD(' ',255)
                                         ELSE CASE WHEN d.rodzaj_dokumentu IN ('WZ', 'ZZ') THEN zbyt_powiazania.F_LISTA_FAKTUR(d.id_dok, pd.pozycja_dokumentu)
                                              ELSE TO_CHAR(NULL)
                                              END
                                         END disp,
                                         CASE WHEN ROWNUM < 0 THEN LPAD(' ',25) ELSE TO_CHAR(NULL) END as intcalcIndeksKontr,
                                         CASE WHEN ROWNUM < 0 THEN LPAD(' ',32) ELSE TO_CHAR(NULL) END as stan,
                                         CASE WHEN ROWNUM < 0 THEN LPAD(' ',32) ELSE TO_CHAR(NULL) END as stan_dostepny,
                                         ILOSC_WYMAGANA_JM_SPRZEDAZY,
                                         nvl(pd.cena * pd.ilosc, 0) wartosc_pozycji_zakup, pd.jm_ceny_zakupu, seria, kraj_pochodzenia,
                                         FUN_WART_POZ_DOK(pd.id_dok, pd.pozycja_dokumentu) wartosc_fun, sysdate intcalcDummy,
                                         pd.kod_stawki_akcyzy, pd.stawka_akcyzy, pd.id_pd, p.prawo_przychod, p.prawo_rozchod,
                                         NULL NULL_INTCLC, mod.data data_modyfikacji_poz, mod.uzytkownik modyfikujacy_poz,
                                         pd.procent_rabatu_min, pd.procent_rabatu_max, pd.cena_jm_ceny_zakupu,
                                      pd.cena_w_walucie_jm_ceny_zakupu, pd.wartosc_akcyzy_zawieszonej, i.czy_akcyza_zawieszona,
                                         i.id_grupy, i.kontrola_pozycje_dok_serie, i.czy_pd_numer_seryjny, i.dokladnosc_atrybutow_zapasu,
                                         CASE WHEN ROWNUM < 0 THEN LPAD(' ',255) ELSE TO_CHAR(NULL) END kod_kreskowy,
                                         mcs.nazwa_mcs, pd.kod_stawki_oplaty_paliwowej, pd.stawka_oplaty_paliwowej,
                                         pd.wartosc_oplaty_paliwowej, pd.kod_formy_dostawy, pd.cena_koszt_gosp_odpadami, pd.wart_koszt_gosp_odpadami,
                                         pd.cena_koszt_gosp_odpadami_dewiz, pd.wart_koszt_gosp_odpadami_dewiz, i.edycja_wh,
                                      PD.CENA_DEWIZOWA_ZBYTU_BRUTTO, PD.CENA_DEWIZOWA_SPRZE_BRUTTO, pd.cena_sprzedazy_brutto
                                      ,pd.numer_transakcji,
                                         pd.WARTOSC_BRUTTO,
                                         pd.WARTOSC_DEWIZ,
                                         pd.WARTOSC_DEWIZ_BRUTTO,
                                         pd.WARTOSC_DEWIZ_VAT,
                                         pd.WARTOSC_NETTO,
                                         pd.WARTOSC_PODATKU_VAT,
                                         pd.CZY_CENA_WG_KOMPETENCJI,
                                         PD.ID_RODZ_TAB_KURS_WAL_SPRZED,
                                         PD.DATA_KURSU_WALUTY_SPRZEDAZY,
                                pd.WARTOSC_WYLACZENIA,pd.WARTOSC_WYLACZENIA_DEWIZ, pd.id_psz,
                                         pd.WARTOSC_EDYTOWANA_DEWIZ, (select ik.INDEKS_CZESCI_KONTRAHENTA from BAZA_BPSC.INDEKSY_DLA_KONTRAHENTOW ik where ik.INDEKS_CZESCI= i.INDEKS_CZESCI and ik.NUMER_KONTRAHENTA=d.NUMER_KONTRAHENTA) UEX_PZ_INDEKS_KONTRAH, (select ik.NAZWA_WG_KONTRAHENTA from INDEKSY_DLA_KONTRAHENTOW ik where ik.INDEKS_CZESCI= i.INDEKS_CZESCI and ik.NUMER_KONTRAHENTA=d.NUMER_KONTRAHENTA) UEX_PZ_NAZWAKONTR, pd.ILOSC*pd.CENA_W_WALUCIE UEX_WART_W_WALUCIE, i.numer_katalogowy UEX_NR_KAT
                                FROM
                                pozycje_dokumentow pd,
                                         pozycje_zamowien_klientow pzk,
                                         pozycje_zamowien_zakupu pzz,
                                         dokumenty d,
                                         indeksy i,
                                      miejsca_skladowania mcs,
                                      rezerwacje_towarow rt,
                                (
                                SELECT Bpsc_dst.maprawo('ZBYT','CZY_DM_CENA_ZAKUPU',USER, NULL,:ID_FIRMY,NULL,'T')
                                                   *Bpsc_dst.maprawo('ZBYT','CZY_DM_CENA_ZAKUPU_NA_PRZYCHODACH',USER, NULL,:ID_FIRMY,NULL,'T') prawo_przychod,
                                                 Bpsc_dst.maprawo('ZBYT','CZY_DM_CENA_ZAKUPU',USER, NULL,:ID_FIRMY,NULL,'T')
                                                    *Bpsc_dst.maprawo('ZBYT','CZY_DM_CENA_ZAKUPU_NA_ROZCHODACH',USER, NULL,:ID_FIRMY,NULL,'T') prawo_rozchod
                                FROM
                                dual
                                ) p
                                ,
                                (
                                SELECT NVL (pd1.data_modyfikacji, pd1.data_wprowadzenia) DATA,
                                                 DECODE (pd1.data_modyfikacji,
                                                           NULL, pd1.wprowadzajacy,
                                                           pd1.modyfikujacy
                                                        ) uzytkownik
                                FROM
                                pozycje_dokumentow pd1
                                WHERE
                                pd1.id_pd = substr(
                                                             (SELECT MAX (to_char(NVL (pd3.data_modyfikacji,
                                                                               pd3.data_wprowadzenia
                                                                              ),'RRRR.MM.DD HH24:MI:SS')||pd3.id_pd
                                                                         )
                                                                FROM pozycje_dokumentow pd3
                                                               WHERE pd3.id_dok = :id_dok),20)
                                ) MOD
                                WHERE
                                d.id_dok = :id_dok
                                     AND pd.id_dok = d.id_dok
                                     AND pd.numer_zamowienia = pzk.zk_numer_zamowienia(+)
                                     AND pd.pozycja_zamowienia = pzk.numer_pozycji(+)
                                     AND pd.rt_id_firmy = pzk.id_firmy(+)
                                     AND pd.numer_zamowienia_zz = pzz.numer_zamowienia(+)
                                     AND pd.pozycja_zamowienia_zz = pzz.numer_pozycji(+)
                                     AND pd.rt_id_firmy = pzz.id_firmy(+)
                                     AND pd.indeks_czesci = i.indeks_czesci
                                     AND pd.rt_id_firmy = rt.id_firmy(+)
                                     AND pd.rt_numer_roku = rt.numer_roku(+)
                                     AND pd.rt_numer_rezerwacji = rt.numer_rezerwacji(+)
                                     AND pd.miejsce_skladowania = mcs.kod_miejsca_skladowania(+)
                                ORDER BY
                                lp
            )";

    QSqlQuery qry;
    qry.prepare(queryLiteralDokPoz);
    qry.bindValue(":id_dok",id_dok);
    qry.bindValue(":ID_FIRMY",id_firma);
    qry.exec();

    return qry;
}
