'use strict'

const {Connection} = require('simpledbc')
const fs = require('fs')

var args = process.argv.slice(2)

const table = 'admin'
let db = 'example.db'
if(args.length) {
  db = args[0];
}

if (fs.existsSync(db)) fs.unlinkSync(db) 


function example() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`
  stmt.execute({
    query: query,
    async: true,
  })
  .then(res => {
    if(res) {
      insert(conn)
      select(conn)
      select2(conn)
      update(conn)
      remove(conn)
      removeAsync(conn);
    }
  })
  .catch(err => {
    console.log(err);
  });
}

function insert(conn) {
  for(var i = 0; i < 99; i++) {
    let stmt = conn.createStatement();
    let password = Math.round(Math.random() * 1000);
    let query =  `INSERT INTO ${table} (passwd, date) VALUES(${password},datetime(\'now\',\'localtime\'));`;
    stmt.execute({
      query: query,
      async: false,
    })
    .then(res => {
      // console.log(res);
      if(res) {
        console.log(`password ${password} inserted`);
      }
    })
    .catch(err => {
      console.log(err);
    });
  }
}

// SELECT
function select(conn) {
  let stmt = conn.createStatement();
  let query =`SELECT idx, passwd, date FROM ${table}`;
  stmt.executeQuery({
    query: query,
    async: true,
  })
  .then(res => {
    while(res.next()) {
      console.log(res.data);
    }
  })
  .catch(err => {
    console.log(err);
  });
}

function select2(conn) {
  let stmt = conn.createStatement();
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

function update(conn) {
  let stmt = conn.createStatement();
  let password = 'new password';
  let query = `UPDATE ${table} set passwd=\'${password}\', date=datetime(\'now\',\'localtime\') WHERE idx=1;`;
  stmt.execute({
    query: query,
    async: true,
  })
  .then(res => {
    if(res) {
      console.log(`successfully updated to ${password}`);
    }
  })
  .catch(err => {
    console.log(err);
  });
}


function remove(conn) {
  let id = 1;
  let stmt = conn.createStatement();
  let query = `DELETE FROM admin WHERE idx=${id};`;
  stmt.execute({
    query: query,
    async: true,
  })
  .then(res => {
    if(res) {
      console.log(`[${id}] deleted successfully!`);
    }
  })
  .catch(err => {
    console.log(err);
  });
}

async function removeAsync(conn) {
  let id = 2;
  let stmt = conn.createStatement();
  let query = `DELETE FROM admin WHERE idx=${id};`;
  let res = await stmt.execute({
    query: query,
    async: true,
  })
  console.log(`removeAsync ${res}`);
}

example();