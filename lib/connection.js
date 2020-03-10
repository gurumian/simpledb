"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _Connection = require('bindings')('simpledbc')._Connection;
var statement_1 = require("./statement");
var prepared_statement_1 = require("./prepared_statement");
/**
 * Class representing a connection
 */
var Connection = /** @class */ (function () {
    /**
     * Create a connection.
     * @param {string} uri - database file path
     * @example
     * let conn = new Connection(':memory:')
     */
    function Connection(uri) {
        this.connection = new _Connection(uri);
    }
    /**
     * Create an instance of Statement
     * @returns {Statement} A constructor that will be instantiated.
     * @example
     *
     * let stmt = conn.createStatement()
     */
    Connection.prototype.createStatement = function () {
        return new statement_1.Statement(this.connection);
    };
    /**
     * Create an instance of PreparedStatement
     * @param {string} query - a query string
     * @returns {PreparedStatement} A constructor that will be instantiated.
     * @example
     *
     * let stmt = conn.prepareStatement('INSERT...')
     */
    Connection.prototype.prepareStatement = function (query) {
        return new prepared_statement_1.PreparedStatement(this.connection, query);
    };
    return Connection;
}());
exports.Connection = Connection;
module.exports = Connection;
//# sourceMappingURL=connection.js.map