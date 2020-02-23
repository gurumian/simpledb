const {Connection} = require('..');

var chai = require('chai');
var assert = chai.assert;    // Using Assert style
var expect = chai.expect;    // Using Expect style
var should = chai.should();  // Using Should style

const fs = require('fs')

const db = './test/example.db'

describe('All', function() {

  if (fs.existsSync(db)) {
    fs.unlinkSync(db);
  }

  describe('Connection', function() {
    let connection = new Connection(db);
    it('should return -1 when create() is failed', function() {
      let stmt = connection.createStatement();
      stmt.execute('CREATE TABLE admin(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);')
      .then(res => {
        assert.equal(res, true);
      });
    });

    it('should return -1 when insert() is failed', function() {
      let stmt = connection.prepareStatement('INSERT INTO admin (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));');
      stmt.setString({
        index: 1,
        value: 'admin_passwd',
      });

      stmt.execute().then(res => {
        assert.equal(res, true);
      });
    });

    it('should return -1 when select() is failed', function() {
      let stmt = connection.createStatement();
      stmt.executeQuery('SELECT idx, passwd, date FROM admin').then(res => {
        assert.equal(res.next(), true);
        assert.equal(res.getInt(0), 1);
        // {
        //   console.log(`${res.getInt(0)}, ${res.getString(1)}, ${res.getString(2)}`);
        // }
      });
    });

    it('should return -1 when update() is failed', function() {
      let stmt = connection.prepareStatement('UPDATE admin set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;');
      stmt.setString({
        index: 1,
        value: 'passcode',
      });
      stmt.execute().then(res =>{
        assert.equal(res, true);
      });
    });


    it('should return -1 when delete() is failed', function() {
      let stmt = connection.prepareStatement('DELETE FROM admin WHERE idx=?;');
      let id = 1;
      stmt.setInt({
        index: 1,
        value: id,
      });
      stmt.execute().then(res => {
        assert.equal(res, true);
      });
    });
  });
});