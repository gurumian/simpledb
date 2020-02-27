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

async function _delete() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let id = 1
  let query = `DELETE FROM ${table} WHERE idx=${id};`;
  let res = await stmt.execute(query)

  if(res) {
    console.log('successfully deleted! run node 4_select.js to see if it\'s done');
  }
}

_delete();