'use strict'

const {Connection} = require('simpledbc')
const fs = require('fs')

var args = process.argv.slice(2)

const table = 'admin'
let db = 'example.db'
if(args.length) {
  db = args[0];
}

if (! fs.existsSync(db)) {
  console.log(`${db} doesn't exist`)
  return;
}  

async function pragma() {
  let conn = new Connection(db)
  let stmt = conn.createStatement();
  let query = `PRAGMA table_info(${table});`;
  let res = await stmt.executeQuery({
    query: query,
  })

  while(res.next()) {
    console.log(res.obj)
  }
}

pragma();
