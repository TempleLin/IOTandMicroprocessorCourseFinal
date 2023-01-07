const sqlite3 = require('sqlite3');

class AppDAO {
    constructor(dbFilePath) {
        this.db = new sqlite3.Database(dbFilePath, (err) => {
            if (err) {
                console.log('Could not connect to database', err)
            } else {
                console.log('Connected to database')
            }
        })
    }

    createTable(tableName, colsAttribs) {
        const sql =
            `
            CREATE TABLE IF NOT EXISTS ${tableName} (
                ${colsAttribs[0]},
                ${colsAttribs[1]}
            )
            `;
        this.tableName = tableName;
        return this.run(sql);
    }

    insertNew(col, values) {
        const sql =
            `
            INSERT INTO ${this.tableName} (${col}) VALUES (?)
            `
        return this.run(sql, values);
    }

    async insertIfNotExists(col, values) {
        let recordIfExists = await this.getWhere(col, values);
        if (recordIfExists !== undefined) {
            return new Promise((resolve, reject) => {
                reject(`TO INSERT ALREADY EXISTS: ${col} ${values}`);
            });
        }
        return this.insertNew(col, values);
    }

    getWhere(col, values) {
        return this.get(`SELECT * FROM ${this.tableName} WHERE ${col} = ?`, values);
    }

    get(sql, params = []) {
        return new Promise((resolve, reject) => {
            this.db.get(sql, params, (err, result) => {
                if (err) {
                    console.log('Error running sql: ' + sql)
                    console.log(err)
                    reject(err)
                } else {
                    resolve(result)
                }
            })
        })
    }

    run(sql, params = []) {
        return new Promise((resolve, reject) => {
            this.db.run(sql, params, function (err) {
                if (err) {
                    console.log('Error running sql ' + sql)
                    console.log(err)
                    reject(err)
                } else {
                    resolve({ id: this.lastID })
                }
            })
        })
    }

    // closeDB() {
    //     this.db.close();
    // }
}

module.exports = AppDAO;