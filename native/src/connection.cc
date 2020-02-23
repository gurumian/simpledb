#include "simpledb.h"
#include <cassert>

namespace util {
namespace db {

Connection::Connection(const std::string &db_path){
  static bool initialized_{false};
  if(! initialized_) {
    sqlite3_initialize();
    initialized_=true;
  }

  int err{0};
  err = sqlite3_open_v2(db_path.c_str(), &conn_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  assert(!err);
}

Connection::~Connection() {
  if(conn_)  sqlite3_close_v2(conn_);
  conn_ = nullptr;
}

std::unique_ptr<Statement> Connection::CreateStatement(void) {
  std::unique_ptr<Statement> stmt(new Statement(conn_));
  return stmt;
}

std::unique_ptr<PreparedStatement> Connection::PrepareStatement(const std::string &sql){
  std::unique_ptr<PreparedStatement> stmt(new PreparedStatement(conn_, sql));
  return std::move(stmt);
}

} // namespace db

} // namespace util