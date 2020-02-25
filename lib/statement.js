const {_Connection} = require('bindings')('simpledbc');

module.exports = class Statement {
  constructor(connection) {
    this.connection = connection;
    this.stmt = connection.createStatement();
  }

  async execute(arg) {
    let query = '';
    let async = true;

    if(typeof arg === 'object') {
      if(! arg.hasOwnProperty('query')) {
        throw "query string or {query: query, async: true} are valid parameters!";
      }

      query = arg.query;

      if(arg.hasOwnProperty('async')) {
        if(typeof arg.async !== 'boolean') {
          throw "query string or {query: query, async: true} are valid parameters!";
        }
        async = arg.async;
      }
    }
    else if(typeof arg === 'string') {
      query = arg;
    }


    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(() => {
          this.stmt.execute(query).then((res, rej) => {
            if(res) resolve(res)
            else reject(rej)
          });
        });
      });
    }

    return this.stmt.execute(query);
  }

  async executeQuery(arg) {
    let query = '';
    let async = true;

    if(typeof arg === 'object') {
      if(! arg.hasOwnProperty('query')) {
        throw "query string or {query: query, async: true} are valid parameters!";
      }

      query = arg.query;

      if(arg.hasOwnProperty('async')) {
        if(typeof arg.async !== 'boolean') {
          throw "query string or {query: query, async: true} are valid parameters!";
        }
        async = arg.async;
      }
    }
    else if(typeof arg === 'string') {
      query = arg;
    }


    if(async) {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.executeQuery(query).then((res, rej) => {
            if(res) resolve(res)
            else reject(rej)
          });
        });
      });
    }

    return this.stmt.executeQuery(query);
  }
}