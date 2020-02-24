'use strict'

var args = process.argv.slice(2);
console.log('args: ', args);


const {Connection} = require('simpledbc');

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
  let query = `CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`;
  stmt.execute(query)
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
    let stmt = connection.createStatement();
    let query =  `INSERT INTO ${table} (passwd, date) VALUES(${Math.random()},datetime(\'now\',\'localtime\'));`;
    stmt.execute(query)
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
  let query =`SELECT idx, passwd, date FROM ${table}`;
  stmt.executeQuery(query)
  .then(res => {
    while(res.next()) {
      console.log(res.data);
    }
  })
  .catch(err => {
    console.log(err);
  });
}

function select2() {
  let stmt = connection.createStatement();
  let query =`SELECT idx, passwd, date FROM ${table}`;
  stmt.executeQuery(query)
  .then(res => {
    while(res.next()) {
      console.log(res.obj);
    }
  })
  .catch(err => {
    console.log(err);
  });
}

// UPDATE
function update() {
  let stmt = connection.createStatement();
  let password = 'new password';
  let query = `UPDATE ${table} set passwd=\'${password}\', date=datetime(\'now\',\'localtime\') WHERE idx=1;`;
  stmt.execute(query)
  .then(res =>{
    console.log(res);
  })
  .catch(err => {
    console.log(err);
  });
}


// DELETE id=1
function remove() {
  let id = 1;
  let stmt = connection.createStatement();
  let query = `DELETE FROM admin WHERE idx=${id};`;
  stmt.execute(query)
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
select2();