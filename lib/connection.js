const {_Connection} = require('bindings')('simpledbc');
const Statement = require('./statement');
const PreparedStatement = require('./prepared_statement');

module.exports = class Connection {
  constructor(uri) {
    this.connection = new _Connection(uri);
  }

  createStatement() {
    return new Statement(this.connection);
  }

  prepareStatement(query) {
    return new PreparedStatement(this.connection, query);
  }
}


