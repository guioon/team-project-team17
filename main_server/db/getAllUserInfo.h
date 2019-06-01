#ifndef __GETALLUSERINFO_H__
#define __GETALLUSERINFO_H__

#include "socket_server.h"
#include "../../structs/struct.h"
#include </usr/include/mysql/mysql.h>
#include <map>

#define HOST "gamehaeduo-db.c8xdbny5rkis.ap-northeast-2.rds.amazonaws.com"
#define USERNAME "gamehaeduo"
#define PASSWORD "caugamehaeduo"
#define DBNAME "gamehaeduo"
#define PORTNUM 3306

using namespace std;



class GetAllUserInfo {
	private :
		// variables for getCharacterInfo
		

		MYSQL conn;
		MYSQL *connection = NULL;
		int query_state;
		char query[255];
		MYSQL_RES *sql_result;
		MYSQL_ROW sql_row;

		// variables for socket

	public :
		// Constructor

		user_character_info user_info;
		
		user_character_info getUserCharacterInfo(string nickname) {
			cout << "Get All Information about Game" << endl;

			// Connect
			mysql_init(&conn);
			mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");		//euckr
			mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

			connection = mysql_real_connect(&conn, HOST, USERNAME, PASSWORD, DBNAME, PORTNUM, NULL, 0);

			if (connection == NULL) {
				cout << "DB Not Connected : " << mysql_error(&conn) << endl;
				return user_info;
			}
		
			// Query - userEntireInfo
			sprintf(query, "SELECT * FROM userEntireInfo WHERE nickname='%s' LIMIT 1", nickname);

			query_state = mysql_query(connection, query);

			if (query_state != 0) {
				cout << "getting user entire information failed : " << mysql_error(&conn) << endl;
				mysql_close(&conn);
				return user_info;
			}

			// Result - userEntireInfo
			sql_result = mysql_store_result(connection);

			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {

				strcpy(user_info.rank , sql_row[1]);
				user_info.wins = atof(sql_row[2]);
				user_info.losses = atof(sql_row[3]);
			}
			mysql_free_result(sql_result);


			// Query - userCharacterInfo
			sprintf(query, "SELECT * FROM userCharacterInfo WHERE nickname='%s'", nickname);

			query_state = mysql_query(connection, query);

			if (query_state != 0) {
				cout << "getting user character information failed : " << mysql_error(&conn) << endl;
				mysql_close(&conn);
				return user_info;
			}

			// Result
			sql_result = mysql_store_result(connection);

			map<char[32], character_info> character_map;
			char ch_key[32];
			character_info ch_value;

			while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {

				strcpy(ch_key ,sql_row[2]);
				ch_value.wins = atof(sql_row[3]);
				ch_value.losses = atof(sql_row[4]);
				ch_value.kills = atof(sql_row[5]);
				ch_value.deaths = atof(sql_row[6]);
				ch_value.assist = atof(sql_row[7]);

				character_map.insert(pair<char[32], character_info>(ch_key, ch_value));
			}

			if (character_map.size() > 0) {
				user_info.character.insert(character_map.begin(), character_map.end());
			}

			// Close
			mysql_free_result(sql_result);
			mysql_close(&conn);

			return user_info;
		}
};

#endif // __GETALLUSERINFO_H__
