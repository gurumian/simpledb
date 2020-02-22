/*
 * prepared_statement.cc
 *
 *  Created on: Mar 2, 2018
 *      Author: buttonfly
 */

#include "simpledb.h"
#include "log_message.h"
// #include <glog/logging.h>
#include <unistd.h>

namespace util {
namespace db {

PreparedStatement::PreparedStatement(sqlite3* connection, const std::string &sql) : Statement(connection) {
  int err = Prepare(sql);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

PreparedStatement::~PreparedStatement() {
}

void PreparedStatement::SetString(int index, const std::string &value) {
  int err = sqlite3_bind_text(stmt_, index, value.c_str(), value.length(), SQLITE_STATIC);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetInt(int index, int value) {
  int err = sqlite3_bind_int(stmt_, index, value);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetInt64(int index, int64_t value) {
  int err = sqlite3_bind_int64(stmt_, index, value);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetDouble(int index, double value) {
  int err = sqlite3_bind_double(stmt_, index, value);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}

void PreparedStatement::SetBlob(int index, void *value, size_t length) {
  int err = sqlite3_bind_blob(stmt_, index, value, length, SQLITE_STATIC);
  if(err) {
    LOG(FATAL) << " error(" << err << ")";
  }
}


bool PreparedStatement::Execute(void) throw (SQLException) {
  return ExecuteUpdate();
}

bool PreparedStatement::ExecuteUpdate(void) throw (SQLException) {
  for(int ret = 0; (ret = sqlite3_step(stmt_)) != SQLITE_DONE;) {
    if(ret == SQLITE_BUSY) {
      sleep(1);
      LOG(INFO) << " busy";
    }
    else {
      return false;
    }
  }
  return true;
}

std::unique_ptr<ResultSet>  PreparedStatement::ExecuteQuery(void) throw (SQLException) {
  std::unique_ptr<ResultSet> result(new ResultSet(stmt_));
  return std::move(result);
}

} // namespace db
} // namespace util