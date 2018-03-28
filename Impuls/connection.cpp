#include "connection.h"

/* Null, because instance will be initialized on demand. */
Connection* Connection::instance = 0;

Connection* Connection::getConnection()
{
    if (instance == 0){
        instance = new Connection();
    }
    return instance;
}


Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("192.168.1.243:1521/TEST");
    db.setDatabaseName("Flowair test");
    db.setUserName("GMICKOWSKI");
    db.setPassword("GMi2016!");
    db.setPort(1521);
    db.open();
}
