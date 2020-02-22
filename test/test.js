const {Connection} = require('..');

var chai = require('chai');
var assert = chai.assert;    // Using Assert style
var expect = chai.expect;    // Using Expect style
var should = chai.should();  // Using Should style

const fs = require('fs')

// modify this properly

describe('All', function() {
  if (! fs.existsSync(uri)) {
    console.log(`No such file: ${uri}. Please modify it`);
    return;
  }

  describe('Connection', function() {
    let connection = new Connection();
    it('should return -1 when the value is not present', function() {
      // source.datasource = uri
      // assert.equal(source.datasource, uri);
    });

    it('should return -1 when the value is not present', function() {
      // let fmt = source.prepare();
      // assert.equal(typeof fmt, 'object');
    });

    // TODO:
  });
});