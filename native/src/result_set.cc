#include "simpledb.h"
#include <string.h>
#include <algorithm>
#include "log_message.h"

namespace util {
namespace db {

ResultSet::ResultSet(std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> stmt) {
  stmt_ = std::move(stmt);
}

bool ResultSet::Next(void) {
  if(sqlite3_step(stmt_.get()) == SQLITE_ROW){
    return true;
  }
  return false;
}

int ResultSet::GetInt(int i){
  return sqlite3_column_int(stmt_.get(), i);
}

const char* ResultSet::GetString(int i){
  return (const char*)sqlite3_column_text(stmt_.get(), i);
}

int64_t ResultSet::GetInt64(int i) {
  return sqlite3_column_int64(stmt_.get(), i);
}

double ResultSet::GetDouble(int i) {
  return sqlite3_column_double(stmt_.get(), i);
}

Blob ResultSet::GetBlob(int i) {
  int n = sqlite3_column_bytes(stmt_.get(), i);
  const uint8_t *start = (uint8_t *)sqlite3_column_blob(stmt_.get(), i);
  Blob blob{new std::vector<uint8_t>(start, start + n)};
  return blob;
}

int ResultSet::GetColumeCount() {
  return sqlite3_column_count(stmt_.get());
}

int ResultSet::columnType(int i) {
  return sqlite3_column_type(stmt_.get(), i);
}

const char *ResultSet::columnName(int i) {
  return sqlite3_column_name(stmt_.get(), i);
}

ResultSet *ResultSet::Unref() {
  return new ResultSet(std::move(stmt_));
}

} // namespace db
} // namespace util