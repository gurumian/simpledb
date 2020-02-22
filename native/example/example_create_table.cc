/*
 * example_create_table.cc
 *
 *  Created on: Mar 3, 2018
 *      Author: buttonfly
 */

#include "simpledb.h"
#include "log_message.h"
// #include <glog/logging.h>
#include <string>
#include <unistd.h>

using namespace util::db;

#define SQL_CREATE_TABLE_PRODUCT "CREATE TABLE product(idx INTEGER PRIMARY KEY AUTOINCREMENT, price0 INTEGER, price1 INTEGER, hash UNSIGNED INTEGER, name TEXT, description TEXT, thumbnail TEXT, date DATETIME);"
#define SQL_CREATE_TABLE_BOUND "CREATE TABLE bound(idx INTEGER PRIMARY KEY AUTOINCREMENT, hash UNSIGNED INTEGER, name TEXT, major TEXT, minor TEXT, status INTEGER, date DATETIME);"
#define SQL_CREATE_TABLE_ADMIN "CREATE TABLE admin(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);"


static int CreateTable(const std::string &path, const std::string &query) {
  Connection conn(path);
  try {
   auto stmt = conn.CreateStatement();
   if(!stmt->Execute(query)) {
      LOG(FATAL) << "E: failed to create  " << query;
      return -1;
    }
  }
  catch (SQLException& e) {
    LOG(ERROR) << e.what();
  }
  return 0;
}

int main(int argc, char *argv[]) {
  const std::string db_path = "./example.db";
  unlink(db_path.c_str());

  CreateTable(db_path, SQL_CREATE_TABLE_ADMIN);
  CreateTable(db_path, SQL_CREATE_TABLE_PRODUCT);
  CreateTable(db_path, SQL_CREATE_TABLE_BOUND);
  return 0;
}



