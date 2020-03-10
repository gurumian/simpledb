"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/**
 * Class representing a statement.
 */
var Statement = /** @class */ (function () {
    /**
     * Create a statement.
     *
     * @param {_Connection} connection - a connection instance
     */
    function Statement(connection) {
        this.connection = connection;
        // this.connection = connection
        this.stmt = connection.createStatement();
    }
    Statement.prototype._param = function (arg) {
        var query = '';
        var async = true;
        var exceptionString = 'query string or {query: query, async: true} are valid parameters!';
        if (typeof arg === 'object') {
            if (!arg.hasOwnProperty('query')) {
                throw exceptionString;
            }
            query = arg.query;
            if (arg.hasOwnProperty('async')) {
                if (typeof arg.async !== 'boolean') {
                    throw exceptionString;
                }
                async = arg.async;
            }
        }
        else if (typeof arg === 'string') {
            query = arg;
        }
        return { query: query, async: async };
    };
    /**
     * Execute a query.
     * @param {object|string} arg - a query string
     * @param {string} arg.query - a query string
     * @param {boolean} arg.async - async mode (true|false)
     * @return {Promise} returns a Promise instance. It'd be either boolean or object
     * @example
     * 1)
     * stmt.execute('INSERT...').then(res => {
     *   ...
     * })
     *
     * 2)
     * stmt.execute({
     *   query: 'INSERT...',
     * }).then(res => {
     *   ...
     * })
     *
     * 3)
     * stmt.execute({
     *   query:'INSERT...',
     *   async: false
     * }).then(res => {...})
     *
     */
    Statement.prototype.execute = function (arg) {
        var _this = this;
        var _a = this._param(arg), query = _a.query, async = _a.async;
        if (query.toUpperCase().startsWith('SELECT') || query.toUpperCase().startsWith('PRAGMA'))
            return this.executeQuery(arg);
        if (async) {
            return new Promise(function (resolve, reject) {
                setTimeout(function () {
                    _this.stmt.execute(query).then(function (_resolve, _reject) {
                        if (_resolve)
                            resolve(_resolve);
                        else
                            reject(_reject);
                    });
                });
            });
        }
        return this.stmt.execute(query);
    };
    /**
     * Execute a query.
     * @param {string} arg - a query string
     * @return {Promise} returns object(s)
     * @example
     *
     * 1)
     * stmt.executeQuery('SELECT...').then(res => {
     *   while(res.next()) {
     *     // res.data is way faster than res.obj
     *     console.log(res.data)
     *     console.log(res.obj)
     *   }
     * })
     *
     *
     * 2)
     * stmt.executeQuery({
     *   query: 'SELECT...',
     * }).then(res => {
     *   ...
     * })
     *
     *
     * 3)
     * stmt.executeQuery({
     *   query: 'SELECT...',
     *   async: false
     * }).then(res => {...})
     *
     */
    Statement.prototype.executeQuery = function (arg) {
        var _this = this;
        var _a = this._param(arg), query = _a.query, async = _a.async;
        if (async) {
            return new Promise(function (resolve, reject) {
                setTimeout(function () {
                    _this.stmt.executeQuery(query).then(function (_resolve, _reject) {
                        if (_resolve)
                            resolve(_resolve);
                        else
                            reject(_reject);
                    });
                });
            });
        }
        return this.stmt.executeQuery(query);
    };
    return Statement;
}());
exports.Statement = Statement;
// module.exports = Statement
//# sourceMappingURL=statement.js.map