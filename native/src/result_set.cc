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

int ResultSet::GetInt(int index){
  return sqlite3_column_int(stmt_.get(), index);
}

const char* ResultSet::GetString(int index){
  return (const char*)sqlite3_column_text(stmt_.get(), index);
}

int64_t ResultSet::GetInt64(int index) {
  return sqlite3_column_int64(stmt_.get(), index);
}

double ResultSet::GetDouble(int index) {
  return sqlite3_column_double(stmt_.get(), index);
}

Blob ResultSet::GetBlob(int index) {
  int n = sqlite3_column_bytes(stmt_.get(), index);
  const uint8_t *start = (uint8_t *)sqlite3_column_blob(stmt_.get(), index);
  Blob blob{new std::vector<uint8_t>(start, start + n)};
  return blob;
}

ResultSet *ResultSet::Unref() {
  return new ResultSet(std::move(stmt_));
}

} // namespace db
} // namespace util