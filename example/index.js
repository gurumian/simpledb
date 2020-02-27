'use strict'

const {Connection} = require('simpledbc')
const fs = require('fs')

var args = process.argv.slice(2)

const table = 'admin'
let db = 'example.db'
if(args.length) {
  db = args[0];
}

function create() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`
  return stmt.execute({
    query: query,
    async: true,
  })
}

function select() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `SELECT idx, passwd, date FROM ${table}`;
  return stmt.execute({
    query: query,
  })
}

function update() {
  let stmt = conn.createStatement();
  let password = 'new password';
  let query = `UPDATE ${table} set passwd=\'${password}\', date=datetime(\'now\',\'localtime\') WHERE idx=1;`;
  return stmt.execute({
    query: query,
    async: true,
  })
}

function _delete() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let id = 1
  let query = `DELETE FROM ${table} WHERE idx=${id};`;
  return stmt.execute({
    query: query,
  })
}

if (!fs.existsSync(db)) {
  let res = create();
  if(!res) throw 'error on create'

  console.log(`\"${db}\" has been created!`)
}


let conn = new Connection(db)
let stmt = conn.createStatement()
let passwd = `admin_passwd`;
let query = `INSERT INTO ${table} (passwd, date) VALUES(\'${passwd}\',datetime(\'now\',\'localtime\'));`
stmt.execute({
  query: query,
})
.then(res => {
  if(!res) throw 'error on insert'
  return select()
})
.then(res => {
  while(res.next()) {
    console.log(res.data)
  }

  // update
  if(! update()) throw 'error on update'
  return select()
})
.then(res => {
  console.log('select after update')
  while(res.next()) {
    console.log(res.data)
  }

  // delete
  if( !_delete()) throw 'error on delete'
  return select()
})
.then(res => {
  console.log('select after delete where id=1')
  while(res.next()) {
    console.log(res.data)
  }
})
.catch(err => {
  console.log(err)
}) 