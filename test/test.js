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

  let connection = new Connection(db);

  describe('Connection', function() {
    it('should return false when create() is failed', function() {
      let stmt = connection.createStatement();
      stmt.execute(`CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, dump BLOB, date DATETIME);`)
      .then(res => {
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return false when create() is failed', function() {
      let stmt = connection.createStatement();
      stmt.executeQuery(`PRAGMA table_info(${table});`)
      .then(res => {
        assert.equal(res.next(), true);
        let data = res.data;
        assert.equal(data[0], 0);
        assert.equal(data[1], 'idx');
        assert.equal(data[2], 'INTEGER');
        assert.equal(data[3], 0);
        
        assert.equal(res.next(), true);
        data = res.data;
        assert.equal(data[0], 1);
        assert.equal(data[1], 'passwd');
        assert.equal(data[2], 'TEXT');
        assert.equal(data[3], 0);
        
        assert.equal(res.next(), true);
        data = res.data;
        assert.equal(data[0], 2);
        assert.equal(data[1], 'dump');
        assert.equal(data[2], 'BLOB');
        assert.equal(data[3], 0);

        assert.equal(res.next(), true);
        data = res.data;
        assert.equal(data[0], 3);
        assert.equal(data[1], 'date');
        assert.equal(data[2], 'DATETIME');
        assert.equal(data[3], 0);

        // [ 0, 'idx', 'INTEGER', 0, null, 1 ]
        // [ 1, 'passwd', 'TEXT', 0, null, 0 ]
        // [ 2, 'dump', 'BLOB', 0, null, 0 ]
        // [ 3, 'date', 'DATETIME', 0, null, 0 ]
      })
      .catch(err => {
        console.log(err);
      });
    });

    // PRAGMA table_info(admin)
    it('should return false when insert() is failed', function() {
      let stmt = connection.prepareStatement(`INSERT INTO ${table} (passwd, dump, date) VALUES(?,?,datetime(\'now\',\'localtime\'));`);
      stmt.setString({
        index: 1,
        value: 'admin_passwd',
      });

      const obj = {hello: 'world'};
      let str = JSON.stringify(obj, null, 2);
      let arraybuffer = new ArrayBuffer(str.length * 2);
      var bufView = new Uint16Array(arraybuffer);
      for (var i=0, strLen=str.length; i < strLen; i++) {
        bufView[i] = str.charCodeAt(i);
      }

      // console.log(arraybuffer);
      stmt.setBlob({
        index: 2,
        value: arraybuffer,
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
      stmt.executeQuery(`SELECT idx, passwd, dump, date FROM ${table}`)
      .then(res => {
        assert.equal(res.next(), true);
        let data = res.data;
        assert.equal(data[0], 1);
        assert.equal(data[1], 'admin_passwd');
        // [
        //   1,
        //   'admin_passwd',
        //   ArrayBuffer {
        //     [Uint8Contents]: <7b 00 0a 00 20 00 20 00 22 00 68 00 65 00 6c 00 6c 00 6f 00 22 00 3a 00 20 00 22 00 77 00 6f 00 72 00 6c 00 64 00 22 00 0a 00 7d 00>,
        //     byteLength: 44
        //   },
        //   '2020-02-24 18:16:41'
        // ]
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
        let data = res.data;
        assert.equal(data[0], 1);
        assert.equal(data[1], 'new_passcode');
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return false when delete() is failed', function() {
      let stmt = connection.prepareStatement('DELETE FROM admin WHERE idx=?;');
      let id = 2;
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

    it('should return false when delete() is failed', function() {
      let stmt = connection.createStatement();
      let id = 1;
      stmt.execute(`DELETE FROM admin WHERE idx=${id};`)
      .then(res => {
        assert.equal(res, true);
      })
    });
  });

  describe('Performance', function() {
    it('should return false when insert() is failed', function() {
      for(var i = 0; i < 100; i++) {
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
      }
    });
  });
});