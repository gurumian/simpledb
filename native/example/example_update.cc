#include <string>
#include "simpledb.h"
#include "log_message.h"

using namespace util::db;

static int update_passwd(const std::string &path, const std::string &passwd) {
  Connection conn(path);
  try {
    auto stmt = conn.PrepareStatement("UPDATE admin set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;");
    stmt->SetString(1, passwd);
    stmt->Execute();
  }
  catch(SQLException& e) {
    LOG(ERROR) << e.what();
  }
  return 0;
}

int main(int argc, char *argv[]) {

  std::string path = "example.db";
  if(argv[1])
    path = argv[1];

  update_passwd(path, "1234");
  return 0;
}