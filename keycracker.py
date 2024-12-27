import pyzipper
import sys
import argparse
from tqdm import tqdm

def try_zip_password(zip_file, wordlist_file):
    # Open the ZIP file
    try:
        with open(wordlist_file, 'r') as wordlist:
            passwords = wordlist.readlines()
    except FileNotFoundError:
        print(f"Error: Wordlist file '{wordlist_file}' not found.")
        return

    with pyzipper.ZipFile(zip_file) as zf:
        # Iterate over passwords with progress bar
        for password in tqdm(passwords, desc="Brute-forcing passwords", unit="password"):
            password = password.strip()  # Remove any extra spaces or newlines
            try:
                # Try to extract with the password
                zf.setpassword(password.encode())
                zf.testzip()  # Test if the password works
                print(f"KEY FOUND: [{password}]")
                return
            except RuntimeError:
                continue  # Incorrect password, continue with next one

    # If no password works
    print("KEY NOT FOUND")

def parse_arguments():
    parser = argparse.ArgumentParser(description="Brute-force a ZIP file password using a wordlist.")
    parser.add_argument("zip_file", help="Path to the ZIP file")
    parser.add_argument("-P", "--wordlist", required=True, help="Path to the wordlist file")
    return parser.parse_args()

if __name__ == "__main__":
    # Parse command-line arguments
    args = parse_arguments()

    # Run the password cracking function
    try_zip_password(args.zip_file, args.wordlist)
