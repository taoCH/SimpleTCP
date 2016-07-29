/********************************************************
        File name       : MyDB.cpp
        Author          : CHEN Tao
        Created Time    : 29/07/2016
********************************************************/

#include "MyDB.h"
#include "log.h"

int MYSQL_YES = 1, MYSQL_NO = 0;

MyDB::MyDB()
{
}

MyDB::~MyDB()
{
	if( conn != NULL ) {
		mysql_close( conn );
	}
}

int
MyDB::InitDB( const string &host, const string &user, const string &pwd, const string &db )
{
	conn = mysql_init( NULL );

	if( conn == NULL ) {
                LogError( "mysql_init() : " + (string)mysql_error( conn ) );
                return MYSQL_NO;
        }

	if( !mysql_real_connect( conn, host.c_str(), user.c_str(), pwd.c_str(), db.c_str(), 0, NULL, 0 ) ) {
		LogError( "mysql_reql_connect() : " + (string)mysql_error( conn ) );
		return MYSQL_NO;
	}

	return MYSQL_YES;
}

int
MyDB::executeWithResult( const string &cmdSQL, MYSQL_RES *res )
{
	if( mysql_query( conn, cmdSQL.c_str() ) ) {
		LogError( "mysql_query() : " + (string)mysql_error( conn ) );
		return MYSQL_NO;
	} else {
		res = mysql_use_result( conn );
		if( res != NULL ) {
			return MYSQL_YES;
		} else {
			LogError( "mysql_use_result() : " + (string)mysql_error( conn ) );
 			return MYSQL_NO;
		}
	}
}

int
MyDB::executeWithoutResult( const string &cmdSQL )
{
	if( mysql_query( conn, cmdSQL.c_str() ) ) {
                LogError( "mysql_query() : " + (string)mysql_error( conn ) );
                return MYSQL_NO;
        } else
		return MYSQL_YES;

}


