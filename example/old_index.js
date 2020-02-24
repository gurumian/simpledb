'use strict'

var args = process.argv.slice(2);
console.log('args: ', args);


const {Connection} = require('simpledb');

var db = 'example.db'
if(args.length) {
  db = args[0];
}

const fs = require('fs')
if (fs.existsSync(db)) {
  fs.unlinkSync(db);
}

const table = 'admin';

let connection = new Connection(db);

// CREATE
function create() {
  let stmt = connection.createStatement();
  stmt.execute(`CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`)
  .then(res => {
    console.log(res);
  })
  .catch(err => {
    console.log(err);
  });
}

// INSERT 100
function insert() {
  for(var i = 0; i < 99; i++) {
    let stmt = connection.prepareStatement(`INSERT INTO ${table} (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));`);
    stmt.setString({
      index: 1,
      value: `admin_passwd${i}`,
    });
  
    stmt.execute()
    .then(res => {
      console.log(res);
    })
    .catch(err => {
      console.log(err);
    });
  }
}

// SELECT
function select() {
  let stmt = connection.createStatement();
  stmt.executeQuery('SELECT idx, passwd, date FROM admin')
  .then(res => {
    while(res.next()) {
      console.log(`${res.getInt(0)}, ${res.getString(1)}, ${res.getString(2)}`);
    }
  })
  .catch(err => {
    console.log(err);
  });
}

// UPDATE
function update() {
  let stmt = connection.prepareStatement(`UPDATE ${table} set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;`);
  stmt.setString({
    index: 1,
    value: 'new_passcode',
  });
  stmt.execute()
  .then(res =>{
    console.log(res);
  })
  .catch(err => {
    console.log(err);
  });
}

// DELETE id=1
function remove() {
  let stmt = connection.prepareStatement('DELETE FROM admin WHERE idx=?;');
  let id = 1;
  stmt.setInt({
    index: 1,
    value: id,
  });
  stmt.execute()
  .then(res => {
    console.log(res);
  })
  .catch(err => {
    console.log(err);
  });
}


create();
insert();
select();
update();
remove();

