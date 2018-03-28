#include "query.h"

Query::Query()
{
}

void Query::setQuery(char* query){
    queryLiteral = query;
}

void Query::bindCriteriaVarialbes(){
    for (int i=queryCriterias.length();i>0;i--){
        //tutaj budowanie WHERE clause
    }
}
void Query::bindValue(QString placeholder, QVariant value){
    qtQuery.bindValue(placeholder,value);
}

void Query::prepare(){
    qtQuery.prepare(queryLiteral);
}

void Query::execute(){
    qtQuery.exec();
}

QSqlQuery* Query::getQuery(){
    return &qtQuery;
}
