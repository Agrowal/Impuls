#ifndef QUERY_H
#define QUERY_H
#include <QtSql>
#include "criteria.h"

class Query
{
public:
    Query();

    void setQuery(char* query);
    void setCriteria();
    void bindCriteriaVarialbes();
    void bindValue(QString placeholder, QVariant value);
    void prepare();
    void execute();
    QSqlQuery* getQuery();

private:
    QSqlQuery qtQuery;
    char* queryLiteral;
    QVector <Criteria> queryCriterias;

};

#endif // QUERY_H
