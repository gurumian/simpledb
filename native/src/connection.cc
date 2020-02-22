#include "simpledb.h"

namespace util {
namespace db {

bool Connection::initialized_= false;

Connection::Connection(const std::string &db_path) throw (SQLException){
  if(! initialized_) {
    sqlite3_initialize();
    initialized_=true;
  }

  if(sqlite3_open_v2(db_path.c_str(), &conn_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL ) != SQLITE_OK) {
    throw SQLException();
  }
}

Connection::~Connection() {
  if(conn_)  sqlite3_close_v2(conn_);
  conn_ = nullptr;
}

std::unique_ptr<Statement> Connection::CreateStatement(void) {
  std::unique_ptr<Statement> stmt(new Statement(conn_));
  return std::move(stmt);
}

std::unique_ptr<PreparedStatement> Connection::PrepareStatement(const std::string &sql){
  std::unique_ptr<PreparedStatement> stmt(new PreparedStatement(conn_, sql));
  return std::move(stmt);
}

} // namespace db
} // namespace util