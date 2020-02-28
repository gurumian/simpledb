const {Connection} = require('..');

var chai = require('chai');
var assert = chai.assert;    // Using Assert style
var expect = chai.expect;    // Using Expect style
var should = chai.should();  // Using Should style

const fs = require('fs')

const async = false;
const db = ':memory:'
const table = 'admin'


describe(`Library (async: ${async})`, function() {
  if (fs.existsSync(db)) fs.unlinkSync(db);

  let connection = new Connection(db);

  describe('Create', function() {
    it('should return false when create() is failed', function() {
      let stmt = connection.createStatement();
      let query = `CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, dump BLOB, date DATETIME);`;
      stmt.execute({
        query: query,
        async: async,
      })
      .then(res => {
        assert.equal(res, true);
        assert.equal(fs.existsSync(db), true);
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return false when create() is failed', function() {
      let stmt = connection.createStatement();
      let query = `PRAGMA table_info(${table});`;
      stmt.execute({
        query: query,
        async: async,
      })
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

    it('should return false when create() is failed', function() {
      let stmt = connection.createStatement();
      let query = `PRAGMA table_info(${table});`;
      stmt.execute({
        query: query,
        async: async,
      })
      .then(res => {
        assert.equal(res.next(), true);
        let obj = res.obj;
        assert.equal(obj.hasOwnProperty('name'), true);
        assert.equal(obj.name, 'idx');
        assert.equal(obj.type, 'INTEGER');

        assert.equal(res.next(), true);
        obj = res.obj;
        assert.equal(obj.hasOwnProperty('name'), true);
        assert.equal(obj.name, 'passwd');
        assert.equal(obj.type, 'TEXT');

        assert.equal(res.next(), true);
        obj = res.obj;
        assert.equal(obj.hasOwnProperty('name'), true);
        assert.equal(obj.name, 'dump');
        assert.equal(obj.type, 'BLOB');

        assert.equal(res.next(), true);
        obj = res.obj;
        assert.equal(obj.hasOwnProperty('name'), true);
        assert.equal(obj.name, 'date');
        assert.equal(obj.type, 'DATETIME');
      })
      .catch(err => {
        console.log(err);
      });
    });
  });

  describe('Insert', function() {
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

      stmt.setBlob({
        index: 2,
        value: arraybuffer,
      });

      stmt.execute({
        async: async,
      })
      .then(res => {
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });
  });

  describe('Select', function() {
    it('should return error when select() is failed', function() {
      let stmt = connection.createStatement();
      let query = `SELECT idx, passwd, dump, date FROM ${table}`;
      stmt.execute({
        query: query,
        async: async,
      })
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
  });

  describe('Update', function() {
    it('should return false when update() is failed', function() {
      let stmt = connection.prepareStatement(`UPDATE ${table} set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;`);
      stmt.setString({
        index: 1,
        value: 'new_passcode',
      });
      stmt.execute({
        async: async,
      })
      .then(res => {
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return error when select() is failed', function() {
      let stmt = connection.createStatement();
      let query = `SELECT idx, passwd, date FROM ${table}`;
      stmt.execute({
        query: query,
        async: async,
      })
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
  });

  describe('Delete', function() {
    it('should return false when delete() is failed', function() {
      let stmt = connection.prepareStatement(`DELETE FROM ${table} WHERE idx=?;`);
      let id = 2;
      stmt.setInt({
        index: 1,
        value: id,
      });
      stmt.execute({
        async: async,
      })
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
      let query = `DELETE FROM ${table} WHERE idx=${id};`;
      stmt.execute({
        query: query,
        async: async,
      })
      .then(res => {
        assert.equal(res, true);
      })
    });
  });

  let count = 10;
  describe('Performance', function() {
    it('should return false when insert() is failed', function() {
      for(var i = 0; i < count; i++) {
        let stmt = connection.prepareStatement(`INSERT INTO ${table} (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));`);
        stmt.setString({
          index: 1,
          value: 'admin_passwd',
        });

        stmt.execute({
          async: async,
        })
        .then(res => {
          assert.equal(res, true);
        })
        .catch(err => {
          console.log(err);
        });
      }
    });

    it('should return false when delete() is failed', function() {
      let stmt = connection.createStatement();
      let query = `DELETE FROM ${table};`;
      stmt.execute({
        query: query,
        async: async,
      })
      .then(res => {
        assert.equal(res, true);
      })
      .catch(err => {
        console.log(err);
      });
    });

    it('should return false when insert() is failed', function() {
      for(var i = 0; i < count; i++) {
        let stmt = connection.createStatement();
        let passwd = `admin_passwd${i}`;
        let query = `INSERT INTO ${table} (passwd, date) VALUES(\'${passwd}\',datetime(\'now\',\'localtime\'));`;
        stmt.execute({
          query: query,
          async: async,
        })
        .then(res => {
          assert.equal(res, true);
        })
        .catch(err => {
          console.log(err);
        });
      }
    });
    
    it('should return false when select count(*) is failed', function() {
      let stmt = connection.createStatement();
      let query = `SELECT COUNT(*) FROM ${table};`;
      stmt.executeQuery({
        query: query,
        async: async,
      })
      .then(res => {
        res.next();
        assert.equal(res.data[0], count);
        if (fs.existsSync(db)) fs.unlinkSync(db);
      })
      .catch(err => {
        console.log(err);
      });
    });
  });
});