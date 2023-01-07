const {httpGet} = require("./http_ctrl");
const DB_Ctrl = require("./db_ctrl");
const fs = require("fs");
const path = require("path");
const {setupSerial, serialWrite} = require("./serial_ctrl");

const SERVER_NAME = "http://192.168.4.1";
const FIND_LOGIN_ROUTE = "/last_login/";
const FIND_REG_ROUTE = "/last_register/";
const INTERVAL_PERIOD = 1000; // Trigger callback every 1000 milliseconds interval.

const DB_DIR_PATH = "./db";
const DB_TABLE_NAME = "rfid_registered_users";
const DB_TABLE_RFID_COL_NAME = "rfid";

const SERIAL_PATH = '/dev/ttyS0';
const SERIAL_BAUD_RATE = 9600;
const SERIAL_ERROR_DETECT = true;

let dbCtrl;
let serialPort;

const findLoginUser = () => {
    httpGet(SERVER_NAME + FIND_LOGIN_ROUTE, (chunk) => {
        chunk = chunk.trim();
        if (chunk !== "None" && chunk !== undefined && chunk !== null && chunk !== '') {
            console.log(`Last login input from ESP32: ${chunk}`);
            dbCtrl.getWhere(DB_TABLE_RFID_COL_NAME, chunk)
                .then(r => {
                    if (r !== undefined) {
                        console.log(`Login successful: ${r}`);
                        serialWrite(serialPort, '1Door Open!!\n');
                    } else {
                        console.log(`User does not exist: ${r}`);
                        serialWrite(serialPort, '0ID Does Not Exist!!\n');
                    }
                });
        }
    });
}

const findRegisterUser = () => {
    httpGet(SERVER_NAME + FIND_REG_ROUTE, (chunk) => {
        chunk = chunk.trim();
        if (chunk !== "None" && chunk !== undefined && chunk !== null && chunk !== '') {
            console.log(`Last register input from ESP32: ${chunk}`);
            dbCtrl.insertIfNotExists(DB_TABLE_RFID_COL_NAME, [chunk]).then(r => {
                    console.log(`Registered successful: ${r}`);
                    serialWrite(serialPort, '1Registered!!\n');
            }).catch(err => {
                console.log(`Register error: ${err}`);
                serialWrite(serialPort, '0Register Error!!\n');
            });
        }
    });
};

const intervalActions = () => {
    findLoginUser();
    findRegisterUser();
}

(async function main() {
    if (!fs.existsSync(DB_DIR_PATH)) {
        fs.mkdirSync(DB_DIR_PATH);
    }
    dbCtrl = new DB_Ctrl(path.join(DB_DIR_PATH, 'rfid_users.db'));
    await dbCtrl.createTable(DB_TABLE_NAME,
            [
                "id INTEGER PRIMARY KEY AUTOINCREMENT",
                `${DB_TABLE_RFID_COL_NAME} VARCHAR(100)`
            ]);

    serialPort = setupSerial(SERIAL_PATH, SERIAL_BAUD_RATE, SERIAL_ERROR_DETECT);

    setInterval(intervalActions, INTERVAL_PERIOD);
})();