import pyzipper
from tqdm import tqdm
import argparse
import sys

def brute_force_zip(zip_file, wordlist_file):
    try:
        # Open the zip file using pyzipper.ZipFile
        with pyzipper.ZipFile(zip_file) as zipf:
            # Read wordlist
            with open(wordlist_file, 'r', encoding='utf-8') as wordlist:
                passwords = wordlist.readlines()
                
                # Remove any extra whitespace or newlines from the passwords
                passwords = [password.strip() for password in passwords]

                # Set up tqdm progress bar
                for password in tqdm(passwords, desc="Cracking password", unit="password"):
                    try:
                        # Try to extract any file with the password
                        zipf.setpassword(password.encode('utf-8'))
                        zipf.testzip()  # Test if the password works
                        print(f"KEY FOUND: {password}")
                        return
                    except RuntimeError:  # Incorrect password
                        continue

        print("KEY NOT FOUND")
    
    except Exception as e:
        print(f"Error occurred: {e}")
        sys.exit(1)

def main():
    # Set up argparse for command-line arguments
    parser = argparse.ArgumentParser(description="Brute-force ZIP file password cracker")
    parser.add_argument('zipfile', help="Path to the ZIP file")
    parser.add_argument('-P', '--passwordlist', required=True, help="Path to the password list")

    # Parse arguments
    args = parser.parse_args()

    # Call the brute_force_zip function
    brute_force_zip(args.zipfile, args.passwordlist)

if __name__ == "__main__":
    main()
