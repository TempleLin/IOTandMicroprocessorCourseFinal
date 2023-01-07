const http = require('http');

const serverName = "http://192.168.4.1";
const findLoginRoute = "/last_login/";
const findRegisterRoute = "/last_register";
const intervalPeriod = 1000; // Trigger callback every 1000 milliseconds interval.

const intervalActions = () => {
    /*
     Get last login from ESP32 if exists.
     */
    http.get(serverName + findLoginRoute, (res) => {
        // The response is a Node.js Stream object
        res.setEncoding('utf8');
        res.on('data', (chunk) => {
            chunk = chunk.trim();
            if (chunk !== "None") {
                console.log(`Last login from ESP32: ${chunk}`);
            }
        });
        res.on('end', () => {
            // console.log('No more data in response.');
        });
    }).on('error', (e) => {
        console.error(`Got error: ${e.message}`);
    });

    /*
     Get last register from ESP32 if exists.
     */
    http.get(serverName + findRegisterRoute, (res) => {
        // The response is a Node.js Stream object
        res.setEncoding('utf8');
        res.on('data', (chunk) => {
            chunk = chunk.trim();
            if (chunk !== "None") {
                console.log(`Last register from ESP32: ${chunk}`);
            }
        });
        res.on('end', () => {
            // console.log('No more data in response.');
        });
    }).on('error', (e) => {
        console.error(`Got error: ${e.message}`);
    });
};

(function main() {
    setInterval(intervalActions, intervalPeriod);
})();