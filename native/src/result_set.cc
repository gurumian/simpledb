#include "simpledb.h"
#include <string.h>

namespace util {
namespace db {

ResultSet::ResultSet(sqlite3_stmt* stmt)
: stmt_(stmt) {
}

ResultSet::~ResultSet() {
}

bool ResultSet::Next(void) {
  if(sqlite3_step(stmt_) == SQLITE_ROW){
    return true;
  }
  return false;
}

int ResultSet::GetInt(int index){
  return sqlite3_column_int(stmt_, index);
}

const char* ResultSet::GetString(int index){
  return (const char*)sqlite3_column_text(stmt_, index);
}

int64_t ResultSet::GetInt64(int index) {
  return sqlite3_column_int64(stmt_, index);
}

double ResultSet::GetDouble(int index) {
  return sqlite3_column_double(stmt_, index);
}

Buffer ResultSet::GetBlob(int index) {
  int n = sqlite3_column_bytes(stmt_, index);

  Buffer buf{
      (uint8_t *) malloc(n),
      [](uint8_t *ptr) { if(ptr) { free(ptr);}}
  };
  memset(buf.get(), 0, n);
  memcpy(buf.get(), sqlite3_column_blob(stmt_, index), n);

  return std::move(buf);
}


} // namespace db
} // namespace util