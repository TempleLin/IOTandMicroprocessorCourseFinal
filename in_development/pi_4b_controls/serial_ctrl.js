const { SerialPort } = require('serialport');

/**
 *
 * @param portPath Path for the port. EX. /dev/ttyS0.
 * @param baudRate Baud-rate for the serial. EX. 9600.
 * @param detectErr Whether add global error callbacks for SerialPort events.
 */
const setupSerial = (portPath, baudRate, detectErr) => {
    let serialPort = new SerialPort({ path: portPath, baudRate: baudRate });

    if (detectErr) {
        serialPort.on('error', function (err) {
            console.log('SerialPort Error: ', err);
        });
    }
    return serialPort;
};

const serialWrite = (serialPort, content) => {
    serialPort.write(content, function(err) {
        if (err) {
            return console.log('Error on SerialPort write: ', err.message)
        }
        console.log('SerialPort message written: ', content);
    })
};

module.exports = {
    setupSerial: setupSerial,
    serialWrite: serialWrite,
};
