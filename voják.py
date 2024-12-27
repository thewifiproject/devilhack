import zipfile
import tqdm
import argparse

def brute_force_zip(zip_file, wordlist_file):
    # Open wordlist
    with open(wordlist_file, 'r', encoding='utf-8') as f:
        wordlist = f.readlines()

    # Remove newlines from the wordlist
    wordlist = [word.strip() for word in wordlist]

    # Open ZIP file
    with zipfile.ZipFile(zip_file) as zf:
        # Use tqdm to display progress bar
        for password in tqdm.tqdm(wordlist, desc="Brute-forcing"):
            try:
                # Try extracting with the current password
                zf.setpassword(password.encode('utf-8'))
                zf.testzip()  # If the password is correct, this will not raise an exception
                print(f"KEY FOUND: [{password}]")
                return
            except RuntimeError:
                # Password is incorrect, continue to the next one
                continue

    # If we get here, no password was found
    print("KEY NOT FOUND")

def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser(description="Brute force ZIP file using a wordlist.")
    parser.add_argument("zip_file", help="The path to the ZIP file.")
    parser.add_argument("-P", "--wordlist", required=True, help="The wordlist file to use for password cracking.")
    args = parser.parse_args()

    # Call the brute force function
    brute_force_zip(args.zip_file, args.wordlist)

if __name__ == "__main__":
    main()
