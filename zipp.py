import pyzipper

# Define the name of the zip file and the password
zip_filename = 'empty_protected.zip'
password = 'password1'

# Create an empty password-protected zip file
with pyzipper.AESZipFile(zip_filename, 'w', compression=pyzipper.ZIP_DEFLATED) as zipf:
    zipf.setpassword(password.encode())

print(f"Created an empty password-protected zip file: {zip_filename}")
