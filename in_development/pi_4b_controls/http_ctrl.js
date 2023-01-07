const http = require('http');

const errorCallback = (err) => {
    console.error(`Got error: ${err.message}`);
}

const httpGet = (optionsStr, onDataCallback, onErrorCallback, onEndCallback = null) => {
    http.get(optionsStr, (res) => {
        res.setEncoding('utf-8');
        // '&&' operator: call the function if the function is not null.
        res.on('data', (chunk) => onDataCallback && onDataCallback(chunk));
        res.on('end', () => onEndCallback && onEndCallback());
    }).on('error', (err) => onErrorCallback? onErrorCallback(err) : errorCallback(err));
}

module.exports = {
    httpGet: httpGet,
}