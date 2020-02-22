#include "simpledb.h"

/*
 ============================================================================
 Name        : exampleProgram.c
 Author      : Sungmin Kim
 Version     :
 Copyright   : Your copyright notice
 Description : Uses shared library to print greeting
               To run the resulting executable the LD_LIBRARY_PATH must be
               set to ${project_loc}/libdb/.libs
               Alternatively, libtool creates a wrapper shell script in the
               build directory of this program which can be used to run it.
               Here the script will be called exampleProgram.
 ============================================================================
 */
#include "simpledb.h"
#include "log_message.h"
// #include <glog/logging.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#define BASE64_CODE_HERE  1

using namespace util::db;

#define SQL_CREATE_TABLE_PRODUCT "CREATE TABLE product(idx INTEGER PRIMARY KEY AUTOINCREMENT, price0 INTEGER, price1 INTEGER, hash UNSIGNED INTEGER, name TEXT, description TEXT, thumbnail TEXT, date DATETIME);"
#define SQL_CREATE_TABLE_BOUND "CREATE TABLE bound(idx INTEGER PRIMARY KEY AUTOINCREMENT, hash UNSIGNED INTEGER, name TEXT, major TEXT, minor TEXT, status INTEGER, date DATETIME);"
#define SQL_CREATE_TABLE_ADMIN "CREATE TABLE admin(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);"



/* aaaack but it's fast and const should make it shared text page. */
static const unsigned char pr2six[256] =
{
    /* ASCII table */
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
int Base64decode_len(const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;
    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    return nbytesdecoded + 1;
}
int Base64decode(char *bufplain, const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;
    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;
    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;
    while (nprbytes > 4) {
    *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    bufin += 4;
    nprbytes -= 4;
    }
    /* Note: (nprbytes == 1) would be an error, so just ingore that case */
    if (nprbytes > 1) {
    *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
    *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
    *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }
    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}
static const char basis_64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64encode_len(int len) {
    return ((len + 2) / 3 * 4) + 1;
}

int Base64encode(char *encoded, const char *string, int len) {
    int i;
    char *p;
    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
    *p++ = basis_64[(string[i] >> 2) & 0x3F];
    *p++ = basis_64[((string[i] & 0x3) << 4) |
                    ((int) (string[i + 1] & 0xF0) >> 4)];
    *p++ = basis_64[((string[i + 1] & 0xF) << 2) |
                    ((int) (string[i + 2] & 0xC0) >> 6)];
    *p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
    *p++ = basis_64[(string[i] >> 2) & 0x3F];
    if (i == (len - 1)) {
        *p++ = basis_64[((string[i] & 0x3) << 4)];
        *p++ = '=';
    }
    else {
        *p++ = basis_64[((string[i] & 0x3) << 4) |
                        ((int) (string[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((string[i + 1] & 0xF) << 2)];
    }
    *p++ = '=';
    }
    *p++ = '\0';
    return p - encoded;
}




static int create_default_db_tables(const char* path) {

  Connection conn(path);
  try {
    std::unique_ptr<Statement> stmt = conn.CreateStatement();
    assert(stmt);
    if(!stmt->Execute(SQL_CREATE_TABLE_ADMIN)) {
      return -1;
    }

    stmt = conn.CreateStatement();
    assert(stmt);
    if(!stmt->Execute(SQL_CREATE_TABLE_PRODUCT)) {
      return -1;
    }

    stmt = conn.CreateStatement();
    assert(stmt);
    if(!stmt->Execute(SQL_CREATE_TABLE_BOUND)) {
      return -1;
    }

  }
  catch (SQLException& e) {
    LOG(ERROR) <<  e.what();
    unlink(path);
  }
  return 0;
}

int insert_default_passwd(const char* path) {

  Connection conn(path);
  try {
    auto stmt = conn.PrepareStatement("INSERT INTO admin (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));");
    // 1. sha1
    // 2. base64
    unsigned char sum[20+1];
    memset(sum, 0, sizeof(sum));
    const char* passwd= "admin";
//                sha1((unsigned char *) passwd, strlen(passwd), (unsigned char*)sum);
    char* base64 = (char*) malloc(Base64encode_len(20)+1);
    memset(base64, 0 , Base64encode_len(20)+1);
    Base64encode(base64, (const char*)sum, 20);
    fprintf(stderr, "sha1+base64 : %s\n", base64);
    stmt->SetString(1, base64);
    free(base64);
    stmt->Execute();
  }
  catch(SQLException& e) {
    LOG(ERROR) <<  e.what();
  }
  return 0;
}

static bool list(const char *db_path) {
  LOG(INFO);

  Connection conn(db_path);
  try {
    std::unique_ptr<Statement> stmt = conn.CreateStatement();
    if(!stmt) {
      LOG(ERROR) << __func__ <<  " failed to create statement";
      return true;
    }
    std::unique_ptr<ResultSet> result = stmt->ExecuteQuery("SELECT idx, price0, price1, hash, name, description, thumbnail, date FROM product");
    if(result) {
      while(result->Next()) {
#if BASE64_CODE_HERE
        const char* descr =  result->GetString(5);
        if(descr && strlen(descr) > 0) {
          char* data = new char[Base64decode_len(descr) +1];
          assert(data);
          memset(data, 0, Base64decode_len(descr) +1);
          Base64decode(data, descr);
          fprintf(stderr, "desc: %s\n", data);
          delete [] data;
          data = nullptr;
        }
#else
        res["description"] =  result->GetCString(3);
#endif
        fprintf(stderr, "res: %d, %d, %d, %u, %s, %s, %s\n",
            result->GetInt(0),
            result->GetInt(1),
            result->GetInt(2),
            result->GetInt(3),
            result->GetString(4),
            result->GetString(6),
            result->GetString(7)
            );
      }
    }
  }
  catch(SQLException& e) {
    LOG(ERROR) <<  e.what();
    return true;
  }
  return true;
}


static int auth(const char* path, const std::string &passwd) {
  LOG(INFO);

  Connection conn(path);
  try {
    std::unique_ptr<Statement> stmt = conn.CreateStatement();
    if(!stmt) {
      fprintf(stderr, "failed to CreateStatement\n");
      return  -1;
    }
    char query[512];
    memset(query, 0, sizeof(query));
    sprintf(query, "SELECT passwd, date FROM admin LIMIT 1");
    std::unique_ptr<ResultSet> result = stmt->ExecuteQuery(query);
    if(result != NULL) {
      if(result->Next()) {
        if(passwd.compare(result->GetString(0)) != 0) {
          fprintf(stderr, "Wrong password\n");
          return -1;
        }
      }
    }
  }
  catch(SQLException& e) {
    LOG(ERROR) <<  e.what();
    return -1;
  }
  return 0;
}

int update_passwd(const char* path, const char *passwd) {
  LOG(INFO);
  Connection conn(path);
  try {
    std::unique_ptr<PreparedStatement> stmt = conn.PrepareStatement("UPDATE admin set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;");
    stmt->SetString(1, passwd);
    stmt->Execute();
  }
  catch(SQLException& e) {
    LOG(ERROR) <<  e.what();
  }
  return 0;
}














int main(void) {
  const char *db_path = "./example.db";
  unlink(db_path);
  create_default_db_tables(db_path);
  insert_default_passwd(db_path);
  getchar();
  list(db_path);
  getchar();
  unsigned char sum[20+1];
  memset(sum, 0, sizeof(sum));
  const char* passwd= "admin";
//  sha1((unsigned char *) passwd, strlen(passwd), (unsigned char*)sum);
  char* base64 = (char*) malloc(Base64encode_len(20)+1);
  memset(base64, 0 , Base64encode_len(20)+1);
  Base64encode(base64, (const char*)sum, 20);
  auth(db_path, base64);
  getchar();
  update_passwd(db_path, "1234");
  return 0;
}

