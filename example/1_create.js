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

async function create() {
  let conn = new Connection(db)
  let stmt = conn.createStatement()
  let query = `CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`
  let res = await stmt.execute({
    query: query,
    async: true,
  })
  
  if(res) {
    console.log('successfully created!');
  }
}

create();
