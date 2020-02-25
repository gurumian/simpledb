const {_Connection} = require('bindings')('simpledbc');

module.exports = class Statement {
  constructor(connection) {
    this.connection = connection;
    this.stmt = connection.createStatement();
  }

  async execute(arg) {
    if(typeof arg === 'object') {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.execute(arg.query).then((res, rej) => {
            if(res) resolve(res)
            else reject(rej)
          });
        });
      });
    }
    else if(typeof arg === 'string'){
      return this.stmt.execute(arg);
    }
    else {
      throw "query string or {query: query, async=true} are valid parameters!";
    }
  }

  async executeQuery(arg) {
    if(typeof arg === 'object') {
      return new Promise((resolve, reject) => {
        setTimeout(()=> {
          this.stmt.executeQuery(arg.query).then((res, rej) => {
            if(res) resolve(res)
            else reject(rej)
          });
        });
      });
    }
    else if(typeof arg === 'string'){
      return this.stmt.executeQuery(arg);
    }
    else {
      throw "query string or {query: query, async=true} are valid parameters!";
    }
  }
}