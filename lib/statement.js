const {_Connection} = require('bindings')('simpledbc')

module.exports = class Statement {
  constructor(connection) {
    this.connection = connection
    this.stmt = connection.createStatement()
  }

  _param(arg) {
    let query = ''
    let async = true
    const exceptionString = 'query string or {query: query, async: true} are valid parameters!'

    if(typeof arg === 'object') {
      if(! arg.hasOwnProperty('query')) {
        throw exceptionString
      }

      query = arg.query

      if(arg.hasOwnProperty('async')) {
        if(typeof arg.async !== 'boolean') {
          throw exceptionString
        }
        async = arg.async
      }
    }
    else if(typeof arg === 'string') {
      query = arg
    }

    return {query, async};
  }

  execute(arg) {
    let {query, async} = this._param(arg)

    if(query.toUpperCase().startsWith('SELECT'))
      return this.executeQuery(arg)

    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(() => {
          this.stmt.execute(query).then((_resolve, _reject) => {
            if(_resolve) resolve(_resolve)
            else reject(_reject)
          })
        })
      })
    }

    return this.stmt.execute(query)
  }

  executeQuery(arg) {
    let {query, async} = this._param(arg)

    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.executeQuery(query).then((_resolve, _reject) => {
            if(_resolve) resolve(_resolve)
            else reject(_reject)
          })
        })
      })
    }

    return this.stmt.executeQuery(query)
  }
}