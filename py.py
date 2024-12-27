import pyzipper
import argparse
from tqdm import tqdm

def brute_force_zip(zip_file, wordlist_file):
    try:
        with open(wordlist_file, 'r') as wordlist:
            wordlist_lines = wordlist.readlines()  # Read all lines to count them
            total_words = len(wordlist_lines)  # Total words for the progress bar
            
            # Use tqdm to wrap the wordlist and show progress
            for password in tqdm(wordlist_lines, total=total_words, desc="Cracking zip", ncols=100):
                password = password.strip()
                try:
                    with pyzipper.AESZipFile(zip_file) as zf:
                        zf.pwd = password.encode('utf-8')  # Set the password
                        zf.test()  # Test if the password works
                        print(f"\nKEY FOUND: [{password}]")  # Print if the password is correct
                        return
                except RuntimeError:
                    continue  # Incorrect password, try the next one

        print("\nKEY NOT FOUND")  # If no password is found after trying all
    except FileNotFoundError:
        print(f"ERROR: Wordlist file '{wordlist_file}' not found.")
    except Exception as e:
        print(f"ERROR: {e}")

def main():
    # Setup argument parser
    parser = argparse.ArgumentParser(description="Brute-force cracker for encrypted zip files")
    parser.add_argument('zip_file', help="The path to the zip file to crack")
    parser.add_argument('-P', '--wordlist', required=True, help="Path to the wordlist file")
    
    # Parse arguments
    args = parser.parse_args()

    # Call the brute force function
    brute_force_zip(args.zip_file, args.wordlist)

if __name__ == "__main__":
    main()
