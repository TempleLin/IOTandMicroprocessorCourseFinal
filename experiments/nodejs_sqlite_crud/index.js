const fs = require('fs');
const path = require('path');
const AppDAO = require("./AppDAO");

const dbDirPath = './db';

if (!fs.existsSync(dbDirPath)) {
    fs.mkdirSync(dbDirPath);
}

let appDAO = new AppDAO(path.join(dbDirPath, 'registered_users.db'));

appDAO.createTable()
    .then(() => appDAO.deleteAll())
    .then(() => appDAO.insertNew('asdfb'))
    .then(() => appDAO.insertNew('54321'))
    .then(() => appDAO.insertNew('#@!$'))
    .then(() => appDAO.insertNew('SADFSAD'))
    .then(() => appDAO.getByCardID('#@!$'))
    .then(user => {
        console.log('Get user:');
        console.log(user);
        appDAO.getAll()
            .then(all => {
                console.log(all);
            });
    })
    .then(() => appDAO.close());
