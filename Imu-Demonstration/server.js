const express = require('express');
const SerialPort = require('serialport');
const app = express();

const SERIAL_PORT_NAME = '/dev/cu.usbmodem1411';

const port = new SerialPort(SERIAL_PORT_NAME, {
    baudRate: 9600,
    parser: SerialPort.parsers.readline('\r\n')
});

var currentData;
port.on('data', data => {
    currentData = data.toString();
});

/**
 *
 */
app.use(express.static(__dirname));

/**
 *
 */
app.get('/', (req, res) => {
    res.sendFile(__dirname + "/index.html");
});

/**
 *
 */
app.get('/api/serial', (req, res) => {
    try {
        console.log(currentData);
        let imuData = JSON.parse(currentData);
        res.status(200).json({
            port: SERIAL_PORT_NAME,
            imu: imuData
        });
    } catch(e) {
        res.send(e.message);
    }
});

/**
 *
 */
app.listen(3000, () => {
    console.log('listening');
});
