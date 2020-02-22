/*
 * statement.cc
 *
 *  Created on: Mar 2, 2018
 *      Author: buttonfly
 */

#include "simpledb.h"
#include <unistd.h>
#include "log_message.h"
// #include <glog/logging.h>

namespace util {
namespace db {

Statement::Statement(sqlite3* conn)
: conn_(conn) {
}

Statement::~Statement(){
  if(stmt_)
    sqlite3_finalize(stmt_);
}

bool Statement::Execute(const std::string& sql) throw (SQLException) {
  return ExecuteUpdate(sql);
}

bool Statement::ExecuteUpdate(const std::string &sql) throw (SQLException) {
  int err;
  err = Prepare(sql);
  if(err) {
    return false;
  }

  for(int ret = 0; (ret = sqlite3_step(stmt_)) != SQLITE_DONE;) {
    if(ret == SQLITE_BUSY) {
      sleep(1);
    }
    else {
      return false;
    }
  }
  return true;
}

std::unique_ptr<ResultSet> Statement::ExecuteQuery(const std::string &sql) throw (SQLException) {
  int err;
  err = Prepare(sql);
  if(err) {
    return nullptr;
  }

  std::unique_ptr<ResultSet> result(new ResultSet(stmt_));
  return std::move(result);
}

int Statement::Prepare(const std::string &sql) {
  char* error = NULL;
  int err = sqlite3_prepare_v2((sqlite3*)conn_, sql.c_str(), sql.length(), &stmt_, (const char**)&error);
  if(err) {
    LOG(ERROR) << " E: sqlite3_prepare: " << error;
    if(error) sqlite3_free(error);
  }

  return err;
}

} /* namespace db */
} /* namespace util */
