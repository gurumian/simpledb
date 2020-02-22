#include "simpledb.h"
#include <string.h>

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

Buffer ResultSet::GetBlob(int index) {
  int n = sqlite3_column_bytes(stmt_.get(), index);

  Buffer buf{
      (uint8_t *) malloc(n),
      [](uint8_t *ptr) { if(ptr) { free(ptr);}}
  };
  memset(buf.get(), 0, n);
  memcpy(buf.get(), sqlite3_column_blob(stmt_.get(), index), n);

  return std::move(buf);
}

ResultSet *ResultSet::Clone() {
  return new ResultSet(std::move(stmt_));
}

} // namespace db
} // namespace util