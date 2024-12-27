import os
import zipfile
from getpass import getpass

def create_zip_with_password():
    # Prompt for password
    password = getpass("ENTER PASSWORD: ")

    # Prompt for the output ZIP file name
    output_file = input("ENTER OUTPUT TO SAVE (e.g., example.zip): ")

    # Validate output file extension
    if not output_file.endswith('.zip'):
        print("Error: Output file must have a .zip extension.")
        return

    # Use the current directory
    current_directory = os.getcwd()
    target_path = current_directory

    # Create ZIP file with password protection
    try:
        with zipfile.ZipFile(output_file, 'w') as zipf:
            # Add directory contents to the ZIP file
            for root, dirs, files in os.walk(target_path):
                for file in files:
                    file_path = os.path.join(root, file)
                    arcname = os.path.relpath(file_path, start=current_directory)
                    zipf.write(file_path, arcname)

        # Inform user of success
        print(f"Zip file '{output_file}' created successfully in the current directory.")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    create_zip_with_password()
