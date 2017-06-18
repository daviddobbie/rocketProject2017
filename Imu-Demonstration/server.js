const SerialPort = require('serialport');
var express = require('express');
var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);

const SERIAL_PORT_NAME = '/dev/cu.usbmodem1411';

const port = new SerialPort(SERIAL_PORT_NAME, {
    baudRate: 9600,
    parser: SerialPort.parsers.readline('\r\n')
});

var currentData;
var socket;
io.on('connection', socketConn => {
    socket = socketConn;
});

port.on('data', data => {
    currentData = data.toString();
    console.log(currentData);

    try {
        let imuData = JSON.parse(currentData);
        if (socket) {
            socket.emit('data', {
                port: SERIAL_PORT_NAME,
                imu: imuData
            });
        }
    } catch(e) {
        console.log(e.message);
    }
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
server.listen(3000, () => {
    console.log('listening');
});
