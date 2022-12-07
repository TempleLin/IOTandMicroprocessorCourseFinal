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

    createTable() {
        const sql =
            `
            CREATE TABLE IF NOT EXISTS user_details (
                id INTEGER PRIMARY KEY AUTOINCREMENT, 
                card_id VARCHAR(100)
            )
            `;
        return this.run(sql);
    }

    insertNew(cardID) {
        const sql =
            `
            INSERT INTO user_details (card_id) VALUES (?)
            `
        return this.run(sql, [cardID]);
    }

    getByCardID(cardID) {
        return this.get('SELECT * FROM user_details WHERE card_id = ?', [cardID]);
    }

    getById(id) {
        return this.get('SELECT * FROM user_details WHERE id = ?', [id]);
    }

    getAll() {
        return this.all(`SELECT * FROM user_details`);
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

    all(sql, params = []) {
        return new Promise((resolve, reject) => {
            this.db.all(sql, params, (err, rows) => {
                if (err) {
                    console.log('Error running sql: ' + sql)
                    console.log(err)
                    reject(err)
                } else {
                    resolve(rows)
                }
            })
        })
    }

    delete(id) {
        return this.run(
            `DELETE FROM user_details WHERE id = ?`,
            [id]
        )
    }

    deleteAll() {
        return this.run(
            'DELETE FROM user_details',
            []
        );
    }

    close() {
        this.db.close();
    }
}

module.exports = AppDAO