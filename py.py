import pyzipper
import os
import argparse

# Function to read passwords lazily (for large wordlists)
def read_passwords(wordlist_path, chunk_size=100):
    with open(wordlist_path, 'r', encoding='utf-8', errors='ignore') as f:
        while True:
            chunk = list(islice(f, chunk_size))
            if not chunk:
                break
            yield [line.strip() for line in chunk]

# Brute force for ZIP file
def brute_force_zip(zip_file_path, wordlist_path, extraction_path):
    failed_attempts = []  # List to store failed attempts for one line of output
    with pyzipper.AESZipFile(zip_file_path) as zf:
        with open(wordlist_path, 'r', encoding='utf-8', errors='ignore') as wordlist_file:
            for line in wordlist_file:
                password_str = line.strip()  # Remove any leading/trailing whitespace (e.g., newlines)
                try:
                    zf.pwd = password_str.encode('utf-8')
                    zf.extractall(path=extraction_path)
                    print(f"KEY FOUND: {password_str}")
                    return password_str
                except RuntimeError:
                    failed_attempts.append(password_str)  # Append failed attempt to the list
                    continue
                except Exception as e:
                    print(f"Error for: {password_str}, Error: {e}")
                    continue

    # Print all failed attempts in a single line after trying all passwords
    if failed_attempts:
        print(f"Attempt Failed: {' '.join(failed_attempts)}")  # Join all failed attempts into one line
    else:
        print("KEY NOT FOUND")

# Setting up argparse for command-line usage
def main():
    parser = argparse.ArgumentParser(description='Brute force a ZIP file password using a wordlist.')
    parser.add_argument('-z', '--zip', required=True, help='Path to the ZIP file')
    parser.add_argument('wordlist', help='Path to the wordlist file')
    parser.add_argument('-e', '--extract', help='Path to extract the ZIP file contents')

    args = parser.parse_args()

    # Ensure the wordlist exists
    if not os.path.exists(args.wordlist):
        print(f"Error: Wordlist file '{args.wordlist}' not found.")
        return

    # Ensure the ZIP file exists
    if not os.path.exists(args.zip):
        print(f"Error: ZIP file '{args.zip}' not found.")
        return

    # Set extraction path if not provided
    if not args.extract:
        args.extract = os.path.dirname(args.zip)

    # Perform ZIP cracking
    print(f"[*] Cracking ZIP file: {args.zip} using wordlist: {args.wordlist}")
    brute_force_zip(args.zip, args.wordlist, args.extract)

if __name__ == "__main__":
    main()
