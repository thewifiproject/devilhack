from fpdf import FPDF
from PyPDF2 import PdfReader, PdfWriter

# Function to generate a PDF
def generate_pdf(output_file):
    pdf = FPDF()
    pdf.add_page()
    pdf.set_font("Arial", size=12)
    pdf.cell(200, 10, txt="Sample PDF Content", ln=True, align='C')
    pdf.output(output_file)

# Function to add password protection
def add_password(input_file, output_file, password):
    reader = PdfReader(input_file)
    writer = PdfWriter()

    for page in reader.pages:
        writer.add_page(page)

    writer.encrypt(password)
    with open(output_file, "wb") as f:
        writer.write(f)

if __name__ == "__main__":
    input_password = input("ENTER PASSWORD: ")
    output_file = input("ENTER OUTPUT TO SAVE (example pdf.pdf): ")

    # Temporary file to create the unprotected PDF
    temp_pdf = "temp.pdf"

    try:
        # Generate PDF and protect it
        generate_pdf(temp_pdf)
        add_password(temp_pdf, output_file, input_password)
        print(f"Password-protected PDF saved as {output_file}")
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        # Clean up temporary file
        import os
        if os.path.exists(temp_pdf):
            os.remove(temp_pdf)
