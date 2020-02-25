'use strict';

module.exports = require('bindings')('simpledbc');
module.exports.Connection = require('./lib/connection');