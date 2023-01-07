const http = require('http');
const {httpGet} = require("./http_ctrl");

const serverName = "http://192.168.4.1";
const findLoginRoute = "/last_login/";
const findRegisterRoute = "/last_register";
const intervalPeriod = 1000; // Trigger callback every 1000 milliseconds interval.

const intervalActions = () => {
    // Get last login from ESP32 if exists.
    httpGet(serverName + findLoginRoute, (chunk) => {
        chunk = chunk.trim();
        if (chunk !== "None") {
            console.log(`Last login from ESP32: ${chunk}`);
        }
    });

    // Get last register from ESP32 if exists.
    httpGet(serverName + findRegisterRoute, (chunk) => {
        chunk = chunk.trim();
        if (chunk !== "None") {
            console.log(`Last register from ESP32: ${chunk}`);
        }
    });
};

(function main() {
    setInterval(intervalActions, intervalPeriod);
})();