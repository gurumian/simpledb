
/**
 * Class representing a statement.
 */
export class PreparedStatement {
  // connection: any;
  stmt: any;
  /**
   * Create a prepared statement.
   * @param {_Connection} connection - a connection instance
   * @param {string} string - a query string
   */
  constructor(public connection: any, query: any) {
    // this.connection = connection
    this.stmt = connection.prepareStatement(query)
  }

  _param(obj: any) {
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

  /**
   * Execute a query.
   * @return {Promise.<boolean>} returns a promise instance
   * @example
   *
   * 1)
   * stmt.execute().then(res => {
   *   ...
   * })
   *
   * 2)
   * stmt.execute(false).then(res => {
   *   ...
   * })
   *
   * 3)
   * stmt.execute({
      async: false
    }).then(res => {...})
   */
  execute(arg: any) {
    let async = this._param(arg)
    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(() => {
          this.stmt.execute().then((_resolve: unknown, _reject: any) => {
            if(_resolve) resolve(_resolve)
            else reject(_reject)
          })
        });
      })
    }
    return this.stmt.execute()
  }

  /**
   * Execute a query.
   * @param {object|boolean} - async mode ( true|false or {async: true|false}). You can leave it empty
   * @return {Promise.<boolean>} returns objects
   * @example
   *
   * 1)
   * stmt.executeUpdate().then(res => {
   *   ...
   * })
   *
   * 2)
   * stmt.executeUpdate(false).then(res => {
   *   ...
   * })
   *
   * 3)
   * stmt.executeUpdate({
      async: false
    }).then(res => {...})
   */
  executeUpdate(obj: any) {
    let async = this._param(obj)
    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.executeUpdate().then((_resolve: unknown, _reject: any) => {
            if(_resolve) resolve(_resolve)
            else reject(_reject)
          })
        });
      })
    }
    return this.stmt.executeUpdate()
  }

  /**
   * Bind the value to the index
   * @param {object} arg - e.g. {index: 1, value: 'hello'}
   * @param {number} arg.index - a binding index
   * @param {string} arg.value - a string to bind
   * @example
   * stmt.setString({
   *  index: 1,
   *  value: 'admin_passwd',
   * });
   */
  setString(arg: any) {
    this.stmt.setString(arg)
  }

  /**
   * Bind the value to the index
   * @param {object} arg - e.g. {index: 1, value: 0}
   * @param {number} arg.index - a binding index
   * @param {number} arg.value - a integer value to bind
   * @example
   * stmt.setInt({
   *   index: 1,
   *   value: 0,
   * });
   */
  setInt(arg: any) {
    this.stmt.setInt(arg)
  }

  /**
   * Bind the value to the index
   * @param {object} arg - e.g. {index: 1, value: 0}
   * @param {number} arg.index - a binding index
   * @param {number} arg.value - a integer64 value to bind
   * @example
   * stmt.setInt64({
   *   index: 1,
   *   value: 0,
   * });
   */
  setInt64(arg: any) {
    this.stmt.setInt64(arg)
  }

  /**
   * Bind the value to the index
   * @param {object} arg - e.g. {index: 1, value: 0}
   * @param {number} arg.index a binding index
   * @param {number} arg.value a double type value to bind
   * @example
   * stmt.setDouble({
   *   index: 1,
   *   value: 0.1,
   * });
   */
  setDouble(arg: any) {
    this.stmt.setDouble(arg)
  }

  /**
   * Bind the value to the index
   * @param {object} arg - e.g. {index: 1, value: arrayBuffer}
   * @param {number} arg.index a binding index
   * @param {ArrayBuffer} arg.value a arraybuffer type value to bind
   * @example
   * stmt.setBlob({
   *   index: 1,
   *   value: arrayBuffer,
   * });
   */
  setBlob(arg: object) {
    this.stmt.setBlob(arg)
  }
}

// module.exports = PreparedStatement