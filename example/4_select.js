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

async function select() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `SELECT idx, passwd, date FROM ${table}`;
  let res = await stmt.execute({
    query: query,
  })

  let now = Date.now();
  while(res.next()) {
    let now = Date.now();
    console.log(res.obj)
  }
  console.log(`when using [res.obj], ${Date.now() - now}ms`)


  // the same test using thenable
  setTimeout(() => {select_thenable()})
}

function select_thenable() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `SELECT * FROM ${table}`;
  stmt.execute({
    query: query,
  })
  .then(res => {
    let now = Date.now();
    while(res.next()) {
      console.log(res.data) // It'd be way faster then res.obj.
    }
    console.log(`when using [res.data], ${Date.now() - now}ms`)
  })
}

select()