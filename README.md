## Prerequisites
```bash
apt install libsqlite3-dev

(or brew)
```

You may need to install `cmake-js` as well
```
npm install -g cmake-js
```


## Install
```bash
npm i simpledb
```

## Example
See `example/`

### CREATE
```js
let connection = new Connection(db);
let stmt = connection.createStatement();
stmt.execute(`CREATE TABLE ${table}(idx INTEGER PRIMARY KEY AUTOINCREMENT, passwd TEXT, date DATETIME);`)
.then(res => {
  assert.equal(res, true);
})
.catch(err => {
  console.log(err);
});
```

### INSERT
```js
let stmt = connection.prepareStatement(`INSERT INTO ${table} (passwd, date) VALUES(?,datetime(\'now\',\'localtime\'));`);
stmt.setString({
  index: 1,
  value: 'admin_passwd',
});

stmt.execute()
.then(res => {
  assert.equal(res, true);
})
.catch(err => {
  console.log(err);
});
```

### SELECT
```js
let stmt = connection.createStatement();
stmt.executeQuery(`SELECT idx, passwd, date FROM ${table}`)
.then(res => {
  assert.equal(res.next(), true);
  assert.equal(res.getInt(0), 1);
  assert.equal(res.getString(1), 'admin_passwd');
  // {
  //   console.log(`${res.getInt(0)}, ${res.getString(1)}, ${res.getString(2)}`);
  // }
})
.catch(err => {
  console.log(err);
});
```

### UPDATE
```js
let stmt = connection.prepareStatement(`UPDATE ${table} set passwd=?, date=datetime(\'now\',\'localtime\') WHERE idx=1;`);
stmt.setString({
  index: 1,
  value: 'new_passcode',
});
stmt.execute()
.then(res =>{
  assert.equal(res, true);
})
.catch(err => {
  console.log(err);
});
```

### DELETE
```js
let stmt = connection.prepareStatement('DELETE FROM admin WHERE idx=?;');
let id = 1;
stmt.setInt({
  index: 1,
  value: id,
});
stmt.execute()
.then(res => {
  assert.equal(res, true);
})
.catch(err => {
  console.log(err);
});
```


## (Optional) Native only build
```bash
cd native;
mkdir build; cd build
cmake .. && make
```
