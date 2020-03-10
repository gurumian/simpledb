"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/**
 * Class representing a statement.
 */
var PreparedStatement = /** @class */ (function () {
    /**
     * Create a prepared statement.
     * @param {_Connection} connection - a connection instance
     * @param {string} string - a query string
     */
    function PreparedStatement(connection, query) {
        this.connection = connection;
        // this.connection = connection
        this.stmt = connection.prepareStatement(query);
    }
    PreparedStatement.prototype._param = function (obj) {
        var async = true;
        if (obj) {
            if (typeof obj === 'boolean') {
                async = obj;
            }
            if (typeof obj === 'object') {
                if (obj.hasOwnProperty('async')) {
                    if (typeof obj.async !== 'boolean') {
                        throw "true | false or { async: true | false } are valid parameters!";
                    }
                    async = obj.async;
                }
            }
        }
        return async;
    };
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
    PreparedStatement.prototype.execute = function (arg) {
        var _this = this;
        var async = this._param(arg);
        if (async) {
            return new Promise(function (resolve, reject) {
                setTimeout(function () {
                    _this.stmt.execute().then(function (_resolve, _reject) {
                        if (_resolve)
                            resolve(_resolve);
                        else
                            reject(_reject);
                    });
                });
            });
        }
        return this.stmt.execute();
    };
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
    PreparedStatement.prototype.executeUpdate = function (obj) {
        var _this = this;
        var async = this._param(obj);
        if (async) {
            return new Promise(function (resolve, reject) {
                setTimeout(function () {
                    _this.stmt.executeUpdate().then(function (_resolve, _reject) {
                        if (_resolve)
                            resolve(_resolve);
                        else
                            reject(_reject);
                    });
                });
            });
        }
        return this.stmt.executeUpdate();
    };
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
    PreparedStatement.prototype.setString = function (arg) {
        this.stmt.setString(arg);
    };
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
    PreparedStatement.prototype.setInt = function (arg) {
        this.stmt.setInt(arg);
    };
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
    PreparedStatement.prototype.setInt64 = function (arg) {
        this.stmt.setInt64(arg);
    };
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
    PreparedStatement.prototype.setDouble = function (arg) {
        this.stmt.setDouble(arg);
    };
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
    PreparedStatement.prototype.setBlob = function (arg) {
        this.stmt.setBlob(arg);
    };
    return PreparedStatement;
}());
exports.PreparedStatement = PreparedStatement;
// module.exports = PreparedStatement
//# sourceMappingURL=prepared_statement.js.map