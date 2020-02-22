#ifndef UTIL_DB_SIMPLE_DB_H_
#define UTIL_DB_SIMPLE_DB_H_

#include <iostream>
#include <exception>
#include <sqlite3.h>
#include <string>
#include <memory>
#include <map>
#include <functional>

namespace util {
namespace db {

using Buffer=std::unique_ptr<uint8_t, std::function<void(uint8_t *)>>;

class NullPointerException: public std::exception {
public:
  virtual const char* what() const throw() {
    return "NullPointerException";
  }
};

class IOException: public std::exception {
public:
  virtual const char* what() const throw() {
    return "IOException";
  }
};

class SQLException: public std::exception {
public:
  virtual const char* what() const throw() {
    return "SQLException";
  }
};

class ResultSet;
class Statement;
class PreparedStatement;
class Connection {
public:
  explicit Connection(const std::string &db_path) throw (SQLException);
  virtual ~Connection();
  std::unique_ptr<Statement> CreateStatement(void);
  std::unique_ptr<PreparedStatement> PrepareStatement(const std::string &sql);

private:
  sqlite3* conn_=nullptr;
  static bool initialized_;
};

class Statement {
protected:
  explicit Statement(sqlite3* connection);

public:
  virtual ~Statement();

  friend class Connection;
  bool Execute(const std::string& sql) throw (SQLException);
  bool ExecuteUpdate(const std::string &sql) throw (SQLException);
  std::unique_ptr<ResultSet> ExecuteQuery(const std::string& sql) throw (SQLException);

protected:
  int Prepare(const std::string &sql);

protected:
  const sqlite3* conn_;
  sqlite3_stmt* stmt_=nullptr;
};

class PreparedStatement: public Statement {
protected:
  explicit PreparedStatement(sqlite3* conn, const std::string &sql);


public:
  virtual ~PreparedStatement();

  friend class Connection;
  void SetString(int index, const std::string &value);
  void SetInt(int index, int value);
  void SetInt64(int index, int64_t value);
  void SetDouble(int index, double value);
  void SetBlob(int index, void *value, size_t length);
  bool Execute(void) throw (SQLException);
  bool ExecuteUpdate(void) throw (SQLException);
  std::unique_ptr<ResultSet> ExecuteQuery(void) throw (SQLException);
};

class ResultSet {
public:
  explicit ResultSet(sqlite3_stmt* stmt);
  virtual ~ResultSet();
  bool Next(void);
  int GetInt(int index);
  int64_t GetInt64(int index);
  double GetDouble(int index);
  Buffer GetBlob(int index);
  const char* GetString(int index);

private:
  sqlite3_stmt* stmt_=nullptr;
};

}
}

#endif // UTIL_DB_SIMPLE_DB_H_