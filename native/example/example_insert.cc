/*
 * example_insert.cc
 *
 *  Created on: Mar 3, 2018
 *      Author: buttonfly
 */


#include <string>
#include "simpledb.h"
#include "log_message.h"
// #include <glog/logging.h>

using namespace util::db;

int insert_default_passwd(const std::string &path) {
  Connection conn(path);
  try {
    auto stmt = conn.PrepareStatement("INSERT INTO admin (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));");
    stmt->SetString(1, "admin_passwd");
    stmt->Execute();
  }
  catch(SQLException& e) {
    LOG(ERROR) << e.what();
  }
  return 0;
}


int main(int argc, char *argv[]) {
  std::string path = "example.db";
  if(argv[1])
    path = argv[1];

  insert_default_passwd(path);
  return 0;
}
