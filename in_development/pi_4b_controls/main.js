const {httpGet} = require("./http_ctrl");
const AppDAO = require("./app_dao");
const fs = require("fs");
const path = require("path");

const serverName = "http://192.168.4.1";
const findLoginRoute = "/last_login/";
const findRegisterRoute = "/last_register/";
const intervalPeriod = 1000; // Trigger callback every 1000 milliseconds interval.

const dbDirPath = "./db";
const dbTableName = "rfid_registered_users";
const dbTableRFIDColName = "rfid";

let appDAO;

const findLoginUser = () => {
    httpGet(serverName + findLoginRoute, (chunk) => {
        chunk = chunk.trim();
        if (chunk !== "None" && chunk !== undefined) {
            console.log(`Last login input from ESP32: ${chunk}`);
            appDAO.getWhere(dbTableRFIDColName, chunk)
                .then(r => {
                    if (r !== undefined) {
                        console.log(`Login successful: ${r}`);
                    } else {
                        console.log(`User does not exist: ${r}`);
                    }
                });
        }
    });
}

const findRegisterUser = () => {
    httpGet(serverName + findRegisterRoute, (chunk) => {
        chunk = chunk.trim();
        if (chunk !== "None" && chunk !== undefined) {
            console.log(`Last register input from ESP32: ${chunk}`);
            appDAO.insertIfNotExists(dbTableRFIDColName, [chunk]).then(r => {
                    console.log(`Registered successful: ${r}`);
                }).catch(err => {
                console.log(`Register error: ${err}`);
            });
        }
    });
};

const intervalActions = () => {
    findLoginUser();
    findRegisterUser();
}

(async function main() {
    if (!fs.existsSync(dbDirPath)) {
        fs.mkdirSync(dbDirPath);
    }
    appDAO = new AppDAO(path.join(dbDirPath, 'rfid_users.db'));
    await appDAO.createTable(dbTableName,
            [
                "id INTEGER PRIMARY KEY AUTOINCREMENT",
                `${dbTableRFIDColName} VARCHAR(100)`
            ]);

    setInterval(intervalActions, intervalPeriod);
})();