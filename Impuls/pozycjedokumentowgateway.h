#ifndef POZYCJEDOKUMENTOWGATEWAY_H
#define POZYCJEDOKUMENTOWGATEWAY_H
#include "query.h"

class PozycjeDokumentowGateway
{
public:
    PozycjeDokumentowGateway();
    QSqlQuery* select(int id_dok);
private:
    Query selectPozycjeDok;
};

#endif // POZYCJEDOKUMENTOWGATEWAY_H
