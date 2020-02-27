'use strict'

const {Connection} = require('simpledbc')
const fs = require('fs')

var args = process.argv.slice(2)

const table = 'admin'
let db = 'example.db'
if(args.length) {
  db = args[0];
}

async function create() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`
  return await stmt.execute({
    query: query,
    async: true,
  })
}

async function select() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `SELECT idx, passwd, date FROM ${table}`;
  return await stmt.execute({
    query: query,
  })
}

async function update() {
  let stmt = conn.createStatement();
  let password = 'new password';
  let query = `UPDATE ${table} set passwd=\'${password}\', date=datetime(\'now\',\'localtime\') WHERE idx=1;`;
  return await stmt.execute({
    query: query,
    async: true,
  })
}

async function _delete() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let id = 1
  let query = `DELETE FROM ${table} WHERE idx=${id};`;
  return await stmt.execute({
    query: query,
  })
}

if (!fs.existsSync(db)) {
  let res = create();
  if(!res) throw 'error on create'
}


let conn = new Connection(db)
let stmt = conn.createStatement()
let passwd = `admin_passwd`;
let query = `INSERT INTO ${table} (passwd, date) VALUES(\'${passwd}\',datetime(\'now\',\'localtime\'));`
stmt.execute({
  query: query,
})
.then(res => {
  if(!res) throw 'error on select'
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
  console.log('after update')
  while(res.next()) {
    console.log(res.data)
  }

  // delete
  if( !_delete()) throw 'error on delete'
  return select()
})
.then(res => {
  console.log('after delete')
  while(res.next()) {
    console.log(res.data)
  }
})
.catch(err => {
  console.log(err)
}) 