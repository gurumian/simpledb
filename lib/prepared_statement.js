const {_Connection} = require('bindings')('simpledbc');

module.exports = class PreparedStatement {
  constructor(connection, query) {
    this.connection = connection;
    this.stmt = connection.prepareStatement(query);
  }

  async execute(async=false) {
    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.execute().then((res, rej) => {
            if(res) resolve(res)
            else reject(rej)
          })
        });
      })
    }
    return this.stmt.execute();
  }

  async executeUpdate(async=false) {
    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.executeUpdate().then((res, rej) => {
            if(res) resolve(res)
            else reject(rej)
          })
        });
      })
    }
    return this.stmt.executeUpdate();
  }
  
  setString(obj) {
    this.stmt.setString(obj);
  }

  setInt(obj) {
    this.stmt.setInt(obj);
  }

  setInt64(obj) {
    this.stmt.setInt64(obj);
  }

  setDouble(obj) {
    this.stmt.setDouble(obj);
  }

  setBlob(obj) {
    this.stmt.setBlob(obj);
  }
}