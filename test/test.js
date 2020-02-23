const {Connection} = require('..');

var chai = require('chai');
var assert = chai.assert;    // Using Assert style
var expect = chai.expect;    // Using Expect style
var should = chai.should();  // Using Should style

const fs = require('fs')

const db = './test/example.db'
const table = 'admin'

describe('All', function() {

  if (fs.existsSync(db)) {
    fs.unlinkSync(db);
  }

  describe('Connection', function() {
    let connection = new Connection(db);
    it('should return false when create() is failed', function() {
      let stmt = connection.createStatement();
      stmt.execute(`CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`)
      .then(res => {
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return false when insert() is failed', function() {
      let stmt = connection.prepareStatement(`INSERT INTO ${table} (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));`);
      stmt.setString({
        index: 1,
        value: 'admin_passwd',
      });

      stmt.execute()
      .then(res => {
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return error when select() is failed', function() {
      let stmt = connection.createStatement();
      stmt.executeQuery(`SELECT idx, passwd, date FROM ${table}`)
      .then(res => {
        assert.equal(res.next(), true);
        assert.equal(res.getInt(0), 1);
        // {
        //   console.log(`${res.getInt(0)}, ${res.getString(1)}, ${res.getString(2)}`);
        // }
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return false when update() is failed', function() {
      let stmt = connection.prepareStatement(`UPDATE ${table} set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;`);
      stmt.setString({
        index: 1,
        value: 'new_passcode',
      });
      stmt.execute()
      .then(res =>{
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return error when select() is failed', function() {
      let stmt = connection.createStatement();
      stmt.executeQuery('SELECT idx, passwd, date FROM admin')
      .then(res => {
        assert.equal(res.next(), true);
        assert.equal(res.getInt(0), 1);
        //console.log(res.getString(1));
        assert.equal(res.getString(1), 'new_passcode');
        // {
        //   console.log(`${res.getInt(0)}, ${res.getString(1)}, ${res.getString(2)}`);
        // }
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return false when delete() is failed', function() {
      let stmt = connection.prepareStatement('DELETE FROM admin WHERE idx=?;');
      let id = 1;
      stmt.setInt({
        index: 1,
        value: id,
      });
      stmt.execute()
      .then(res => {
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });

  });
});