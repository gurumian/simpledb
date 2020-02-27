'use strict'

const {Connection} = require('simpledbc')
const fs = require('fs')

var args = process.argv.slice(2)

const table = 'admin'
let db = 'example.db'
if(args.length) {
  db = args[0]
}

if (! fs.existsSync(db)) {
  console.log(`${db} doesn't exist`)
  return;
}  

async function insert() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let passwd = `admin_passwd`;
  let query = `INSERT INTO ${table} (passwd, date) VALUES(\'${passwd}\',datetime(\'now\',\'localtime\'));`
  let res = await stmt.execute({
    query: query,
  })

  if(res) {
    console.log('successfully inserted!');
  }
}

insert();
