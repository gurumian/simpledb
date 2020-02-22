#include <string>
#include "simpledb.h"
#include "log_message.h"

using namespace util::db;

static bool list(const std::string &path) {
  Connection conn(path);
  auto stmt = conn.CreateStatement();
  if(!stmt) {
    LOG(ERROR) << __func__ <<  " failed to create statement";
    return true;
  }

  auto result = stmt->ExecuteQuery("SELECT idx, passwd, date FROM admin");
  if(result) {
    while(result->Next()) {
      fprintf(stderr, "res: %d, %s, %s\n",
          result->GetInt(0),
          result->GetString(1),
          result->GetString(2)
          );
    }
  }
  return true;
}

int main(int argc, char *argv[]) {

  std::string path = "example.db";
  if(argv[1])
    path = argv[1];

  list(path);
  return 0;
}