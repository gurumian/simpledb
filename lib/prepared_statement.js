const {_Connection} = require('bindings')('simpledbc')

module.exports = class PreparedStatement {
  constructor(connection, query) {
    this.connection = connection
    this.stmt = connection.prepareStatement(query)
  }

  _param(obj) {
    let async = true
    if(obj) {
      if(typeof obj === 'boolean') {
        async = obj
      }

      if(typeof obj === 'object') {
        if(obj.hasOwnProperty('async')) {
          if(typeof obj.async !== 'boolean') {
            throw "true | false or { async: true | false } are valid parameters!";
          }
          async = obj.async
        }
      }
    }
    return async
  }

  execute(obj) {
    let async = this._param(obj)
    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(() => {
          this.stmt.execute().then((_resolve, _reject) => {
            if(_resolve) resolve(_resolve)
            else reject(_reject)
          })
        });
      })
    }
    return this.stmt.execute()
  }

  executeUpdate(obj) {
    let async = this._param(obj)
    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.executeUpdate().then((_resolve, _reject) => {
            if(_resolve) resolve(_resolve)
            else reject(_reject)
          })
        });
      })
    }
    return this.stmt.executeUpdate()
  }
  
  setString(obj) {
    this.stmt.setString(obj)
  }

  setInt(obj) {
    this.stmt.setInt(obj)
  }

  setInt64(obj) {
    this.stmt.setInt64(obj)
  }

  setDouble(obj) {
    this.stmt.setDouble(obj)
  }

  setBlob(obj) {
    this.stmt.setBlob(obj)
  }
}