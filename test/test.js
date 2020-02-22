const {Connection} = require('..');

var chai = require('chai');
var assert = chai.assert;    // Using Assert style
var expect = chai.expect;    // Using Expect style
var should = chai.should();  // Using Should style

const fs = require('fs')

const db = './test/example.db'
// modify this properly

describe('All', function() {

  if (fs.existsSync(db)) {
    fs.unlinkSync(db);
    return;
  }

  describe('Connection', function() {
    let connection = new Connection(db);
    it('should return -1 when the value is not present', function() {
      let stat = connection.createStatement();
      let res = stat.execute('CREATE TABLE admin(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);');
      console.log(res);
    });

    it('should return -1 when the value is not present', function() {
      let stat = connection.prepareStatement('INSERT INTO admin (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));');
      stat.setInt({
        index: 1,
        value: 'admin_passwd',
      });

      let res = stat.execute();
      console.log(res);
      assert.equal(res, true);
    });

    it('should return -1 when the value is not present', function() {
      let stat = connection.createStatement();
      let res = stat.executeQuery('SELECT idx, passwd, date FROM admin');
      console.log(res);
      // assert.equal(res, true);
    });

    // TODO:
  });
});



// auto stmt = conn.CreateStatement();
//   if(!stmt) {
//     LOG(ERROR) << __func__ <<  " failed to create statement";
//     return true;
//   }

//   auto result = stmt->ExecuteQuery("SELECT idx, passwd, date FROM admin");
//   if(result) {
//     while(result->Next()) {
//       fprintf(stderr, "res: %d, %s, %s\n",
//           result->GetInt(0),
//           result->GetString(1),
//           result->GetString(2)
//           );
//     }
//   }