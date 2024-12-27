import pyzipper

# Create a password-protected empty ZIP file
zip_file_name = "protected.zip"
password = b"password1"  # Password should be in bytes

with pyzipper.AESZipFile(zip_file_name, 'w', compression=pyzipper.ZIP_DEFLATED) as zipf:
    zipf.setpassword(password)  # Set the password
    # No files are added, so it's just an empty ZIP file protected by the password

print(f"Created {zip_file_name} with password protection.")
