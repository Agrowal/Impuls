#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtCore>
#include <QtSql>


class Connection
{
public:
    // Static access method
    static Connection* getConnection();


private:
    // Private constructor
    Connection();
    // Instance stored
    static Connection* instance;

    QSqlDatabase db;
};

#endif // CONNECTION_H
