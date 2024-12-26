const http = require('http');
const fs = require('fs');
const path = require('path');

const server = http.createServer((req, res) => {
  // Cesta k původnímu souboru
  const filePath = path.join('C:', 'devil', 'dist', 'excel.exe'); // Původní soubor

  // Cesta, kam chceme soubor uložit na disk C
  const savePath = path.join('C:', 'excel.exe'); // Cílový soubor pro uložení

  // Ověření, že požadavek je na URL pro stažení
  if (req.url === '/stahnout') {
    // Zjištění typu souboru podle přípony
    const extname = path.extname(filePath).toLowerCase();
    let contentType = 'application/octet-stream'; // Pro binární soubory

    // Nastavení typu pro .exe soubor
    if (extname === '.exe') {
      contentType = 'application/x-msdownload'; // Nastavení pro .exe soubory
    }

    // Uložit soubor na disk C
    fs.copyFile(filePath, savePath, (err) => {
      if (err) {
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        res.end('Chyba při ukládání souboru na disk C.');
        return;
      }
      
      // Odeslání souboru v odpovědi pro stažení
      fs.readFile(filePath, (err, data) => {
        if (err) {
          res.writeHead(500, { 'Content-Type': 'text/plain' });
          res.end('Chyba při čtení souboru.');
        } else {
          res.writeHead(200, {
            'Content-Type': contentType,
            'Content-Disposition': 'attachment; filename="excel.exe"' // Změna názvu souboru pro stažení
          });
          res.end(data);
        }
      });
    });
  } else {
    // Pokud není požadována URL pro stažení
    res.writeHead(200, { 'Content-Type': 'text/html' });
    res.end('<html><body><a href="/stahnout">ERORR!</a></body></html>');
  }
});

const PORT = 3000;
const LOCAL_IP = '10.0.1.35'; // Zde zadej svou lokální IP (např. 192.168.1.100)

server.listen(PORT, LOCAL_IP, () => {
  console.log(`Server běží na http://${LOCAL_IP}:${PORT}`);
});
