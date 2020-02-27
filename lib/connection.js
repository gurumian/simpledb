const {_Connection} = require('bindings')('simpledbc')
const Statement = require('./statement')
const PreparedStatement = require('./prepared_statement')


/**
 * Class representing a connection
 */
class Connection {
  /**
   * Create a connection.
   * @param {string} uri - database file path
   * @example
   * let conn = new Connection(':memory:')
   */
  constructor(uri) {
    this.connection = new _Connection(uri)
  }

  /**
   * Create an instance of Statement
   * @returns {Statement} A constructor that will be instantiated.
   * @example
   *
   * let stmt = conn.createStatement()
   */
  createStatement() {
    return new Statement(this.connection)
  }

  /**
   * Create an instance of PreparedStatement
   * @param {string} query - a query string
   * @returns {PreparedStatement} A constructor that will be instantiated.
   * @example
   *
   * let stmt = conn.prepareStatement('INSERT...')
   */
  prepareStatement(query) {
    return new PreparedStatement(this.connection, query)
  }
}

module.exports = Connection