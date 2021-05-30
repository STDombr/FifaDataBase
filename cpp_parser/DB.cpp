//
// Created by Vitaliy on 18/04/2020.
//

#include <iostream>
#include <string>
#include "sqlite3.h"
#include "Match.h"
#include <vector>

using namespace std;

extern vector<Match*> matches;

/**
 * Callback function to flii_students that adds the student to the global vector students
 */
static int callback_data(void *not_used, int argc, char **argv, char **azColName){
    string id = argv[0];
    string name = argv[1];
    string group_id = argv[2];

    //Student *temp = new Student(name,group_id,id);
    //students.push_back(temp);

    return 0;
}



static int callback(void *data, int argc, char **argv, char **azColName){
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/**
 * Fills the global vector Students by data in db
 */
void fill_matches() {

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    const char* data = "Callback function called";

    FILE *fin = fopen("../feed.txt","r");
    if(fin == nullptr){
        std::cout << "Wrong file" << std::endl;
        return;
    }

    char buf[200];
    string buf_str, time, team1, player1, player2, team2, score1, score2;

    sqlite3_open("matches.db", &db);

    string query = "INSERT INTO MATCHES (ID, TIME, TEAM1, PLAYER1, PLAYER2, TEAM2, SCORE1, SCORE2)" \
        "VALUES(";
    //"VALUES(1, 'Programmer'); ";

    /*   char str[11];
   int number = 12345;
   sprintf(str, "%d", number);*/

    char id[10];
    long long int num = 0;
    string final;
    string all_queries;
    while (!feof(fin)) {
        //fscanf(fin, "%s", buf);
        fgets(buf,198, fin);
        buf_str = buf;
        //matches.emplace_back(new Match(buf_str));
        auto *temp = new Match(buf_str);

        num++;

        sprintf(id, "%d", num);
        string match_data = id;
        match_data += ", '";
        match_data += temp->time;
        match_data += "', '";
        match_data += temp->team1;
        match_data += "', '";
        match_data += temp->player1;
        match_data += "', '";
        match_data += temp->player2;
        match_data += "', '";
        match_data += temp->team2;
        match_data += "', '";
        match_data += temp->score1;
        match_data += "', '";
        match_data += temp->score2;
        match_data += "'); ";

        final.erase();
        final = query + match_data;

        all_queries += final;

        //cout << sql << endl;

        sql = final.c_str();

        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error3: %s\n", zErrMsg);
            cout << sql;
            sqlite3_free(zErrMsg);
            return;
        } else fprintf(stdout, "Records created successfully\n");


    }


/*    sql = all_queries.c_str();

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error3: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else fprintf(stdout, "Records created successfully\n");*/

    fclose(fin);
    sqlite3_close(db);

    //cout << endl << sql;
    cout << num;
}



void create_db(){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    const char* data = "Callback function called";

    rc = sqlite3_open("matches.db", &db);

    {
        sql = "CREATE TABLE MATCHES("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "TIME           TEXT    NOT NULL," \
      "TEAM1          TEXT    NOT NULL," \
      "PLAYER1        TEXT    NOT NULL,"
      "PLAYER2        TEXT    NOT NULL,"
      "TEAM2          TEXT    NOT NULL,"
      "SCORE1         INT     NOT NULL,"
      "SCORE2         INT     NOT NULL,"
      "SCORE11        INT,"
      "SCORE21        INT)";

        rc = sqlite3_exec(db,sql, callback, 0, &zErrMsg);

        if(rc != SQLITE_OK){
            fprintf(stderr, "SQL error2: %s\n", zErrMsg);
            //cout << "error" << endl;
            sqlite3_free(zErrMsg);
        } else {
            //cout << "success" << endl;
            fprintf(stdout, "Tables created successfully\n");
        }

    }

/*    for(size_t i = 0; i < STUDENTSNUM; i++){
        string temp_name = students1[i];
        string out_name = "'" + temp_name + "'";
        long long int temp_hash = strhash(temp_name);
        int temp_group = rand() % 5 + 1;
        string out = "INSERT INTO STUDENTS (ID,NAME,GROUP_ID) "  \
                     "VALUES (";
        out += std::to_string(temp_hash);
        out += ", ";
        out += out_name + ", ";
        out += std::to_string(temp_group);
        out += ");";
        sql = out.c_str();

        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error3: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "+");
        }
    }*/
    sqlite3_close(db);
}