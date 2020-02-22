#include <string>
#include "simpledb.h"
#include "log_message.h"

using namespace util::db;

int insert_default_passwd(const std::string &path) {
  Connection conn(path);
  auto stmt = conn.PrepareStatement("INSERT INTO admin (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));");
  stmt->SetString(1, "admin_passwd");
  stmt->Execute();
  return 0;
}

int main(int argc, char *argv[]) {
  std::string path = "example.db";
  if(argv[1])
    path = argv[1];

  insert_default_passwd(path);
  return 0;
}