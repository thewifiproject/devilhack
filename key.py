import pyzipper
import rarfile
import py7zr
import argparse
from tqdm import tqdm

def brute_force_zip(zipfile_path, wordlist):
    try:
        with pyzipper.AESZipFile(zipfile_path) as zf:
            for password in tqdm(wordlist, desc="Brute-forcing ZIP"):
                try:
                    zf.setpassword(password)
                    zf.test()  # Try to open the file with the current password
                    print(f"KEY FOUND: {password}")
                    return
                except:
                    continue
            print("KEY NOT FOUND")
    except Exception as e:
        print(f"Error processing ZIP file: {e}")

def brute_force_rar(rarfile_path, wordlist):
    try:
        with rarfile.RarFile(rarfile_path) as rf:
            for password in tqdm(wordlist, desc="Brute-forcing RAR"):
                try:
                    if rf.test(password):
                        print(f"KEY FOUND: {password}")
                        return
                except:
                    continue
            print("KEY NOT FOUND")
    except Exception as e:
        print(f"Error processing RAR file: {e}")

def brute_force_7z(sevenzfile_path, wordlist):
    try:
        with py7zr.SevenZipFile(sevenzfile_path, mode='r') as szf:
            for password in tqdm(wordlist, desc="Brute-forcing 7z"):
                try:
                    szf.setpassword(password)
                    szf.test()  # Try to open the file with the current password
                    print(f"KEY FOUND: {password}")
                    return
                except:
                    continue
            print("KEY NOT FOUND")
    except Exception as e:
        print(f"Error processing 7z file: {e}")

def main():
    # Setup argparse for command-line arguments
    parser = argparse.ArgumentParser(description="Brute force ZIP, RAR, and 7z files using a wordlist.")
    parser.add_argument("file", help="Path to the archive file (zip, rar, or 7z)")
    parser.add_argument("-P", "--wordlist", help="Path to the wordlist file", required=True)

    args = parser.parse_args()

    # Read the wordlist
    try:
        with open(args.wordlist, 'r') as f:
            wordlist = f.read().splitlines()
    except FileNotFoundError:
        print(f"Wordlist file {args.wordlist} not found.")
        return

    # Detect the file type based on the extension
    if args.file.endswith('.zip'):
        brute_force_zip(args.file, wordlist)
    elif args.file.endswith('.rar'):
        brute_force_rar(args.file, wordlist)
    elif args.file.endswith('.7z'):
        brute_force_7z(args.file, wordlist)
    else:
        print("Unsupported file format")

if __name__ == "__main__":
    main()
