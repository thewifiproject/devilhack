from fpdf import FPDF

def create_pdf(password, filename):
    # Create instance of FPDF class
    pdf = FPDF()

    # Add a page to the PDF
    pdf.add_page()

    # Set font for the PDF
    pdf.set_font("Arial", size=12)

    # Add text to the PDF
    pdf.cell(200, 10, txt="Password: " + password, ln=True)

    # Save the PDF with the given filename
    pdf.output(filename)

def main():
    # Get user input for password and output filename
    password = input("ENTER PASSWORD: ")
    filename = input("ENTER OUTPUT TO SAVE (example pdf.pdf): ")

    # Create and save the PDF
    create_pdf(password, filename)
    print(f"PDF has been saved as {filename}")

if __name__ == "__main__":
    main()
