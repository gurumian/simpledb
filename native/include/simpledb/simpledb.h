#ifndef UTIL_DB_SIMPLE_DB_H_
#define UTIL_DB_SIMPLE_DB_H_

#include <iostream>
#include <exception>
#include <sqlite3.h>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <vector>

namespace util {
namespace db {

using Blob=std::unique_ptr<std::vector<uint8_t>>;

class ResultSet;
class Statement;
class PreparedStatement;
class Connection {
public:
  explicit Connection(const std::string &db_path);
  virtual ~Connection();
  std::unique_ptr<Statement> CreateStatement(void);
  std::unique_ptr<PreparedStatement> PrepareStatement(const std::string &sql);

private:
  sqlite3* conn_{};
};

class Statement {
protected:
  explicit Statement(const sqlite3* connection);

protected:
  Statement(const sqlite3* conn, std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> stmt);

public:
  virtual ~Statement()=default;

  friend class Connection;
  bool Execute(const std::string& sql);
  bool ExecuteUpdate(const std::string &sql);
  std::unique_ptr<ResultSet> ExecuteQuery(const std::string& sql);
  Statement *Unref();

protected:
  int Prepare(const std::string &sql);

protected:
  const sqlite3* conn_;
  std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> stmt_{};
};

class PreparedStatement: public Statement {
protected:
  explicit PreparedStatement(const sqlite3* conn, const std::string &sql);

private:
  PreparedStatement(const sqlite3 *conn, std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>>);

public:
  virtual ~PreparedStatement()=default;

  friend class Connection;
  void SetString(int index, const std::string &value);
  void SetInt(int index, int value);
  void SetInt(int index, int64_t value);
  void SetDouble(int index, double value);
  void SetBlob(int index, void *value, size_t length);
  bool Execute(void);
  bool ExecuteUpdate(void);
  std::unique_ptr<ResultSet> ExecuteQuery(void);
  PreparedStatement *Unref();
};

class ResultSet {
public:
  explicit ResultSet(std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> stmt);
  bool Next(void);
  int GetInt(int index);
  int64_t GetInt64(int index);
  double GetDouble(int index);
  Blob GetBlob(int index);
  const char* GetString(int index);
  int GetColumeCount();
  int columnType(int index);
  const char *columnName(int index);
  ResultSet *Unref();

private:
  std::unique_ptr<sqlite3_stmt, std::function<void(sqlite3_stmt *)>> stmt_{};
};

} // db
} // util

#endif // UTIL_DB_SIMPLE_DB_H_