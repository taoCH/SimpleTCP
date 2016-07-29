/********************************************************
	File name	: MyDB.h
	Author		: CHEN Tao
	Created Time 	: 29/07/2016
********************************************************/



#ifndef MYDB_H
#define MYDB_H

#include <iostream>
#include <string>
#include <mysql/mysql.h>

using namespace std;

extern int MYSQL_YES, MYSQL_NO; 

class MyDB {
public:
	MyDB();
	~MyDB();

	int InitDB( const string &host, const string &user, const string &pwd, const string &db );
	int executeWithResult( const string &cmdSQL, MYSQL_RES *res );
	int executeWithoutResult( const string &cmdSQL );
private:
	MYSQL *conn;
};


#endif // MYDB_H
