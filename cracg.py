import pyzipper
import sys
import argparse
import os
import threading
import queue
from tqdm import tqdm  # Import tqdm for progress bar

def zip_bruteforce(zip_file, password_file, num_threads=4):
    # Check if the zip file exists
    if not os.path.isfile(zip_file):
        print(f"NO ZIP FILE NAMED {zip_file} FOUND!")
        sys.exit(1)

    # Open the password file
    try:
        with open(password_file, 'r') as file:
            passwords = file.readlines()
    except FileNotFoundError:
        print(f"Password file '{password_file}' not found.")
        sys.exit(1)

    # Initialize a queue to pass passwords to threads
    password_queue = queue.Queue()

    # Fill the queue with passwords
    for password in passwords:
        password_queue.put(password.strip())

    # Function to try passwords in parallel
    def try_passwords():
        with pyzipper.AESZipFile(zip_file) as zf:
            while not password_queue.empty():
                password = password_queue.get()
                try:
                    # Attempt to extract the first file in the zip
                    zf.setpassword(password.encode())
                    zf.testzip()  # Test if the password works
                    print(f"KEY FOUND: [{password}]")
                    sys.exit(0)  # Stop the program once the password is found
                except RuntimeError:
                    pass

    # Create threads for parallel execution
    threads = []
    for _ in range(num_threads):
        thread = threading.Thread(target=try_passwords)
        thread.start()
        threads.append(thread)

    # Show a progress bar for the number of attempts
    with tqdm(total=len(passwords), desc="Brute-forcing passwords", unit="password") as pbar:
        while any(thread.is_alive() for thread in threads):
            pbar.update(1)  # Increment progress bar
            pbar.set_postfix({'threads': sum(1 for thread in threads if thread.is_alive())})
            pbar.refresh()

        for thread in threads:
            thread.join()

    print("KEY NOT FOUND")

def main():
    # Set up the argument parser
    parser = argparse.ArgumentParser(description="Brute-force a password protected ZIP file using a list of passwords.")
    
    # Define command-line arguments
    parser.add_argument("zip_file", help="The path to the ZIP file to crack.")
    parser.add_argument("-P", "--password_file", required=True, help="The path to the file containing a list of passwords.")
    parser.add_argument("-t", "--threads", type=int, default=4, help="Number of threads to use for parallel brute-forcing.")
    
    # Parse the arguments
    args = parser.parse_args()

    try:
        # Call the zip_bruteforce function
        zip_bruteforce(args.zip_file, args.password_file, args.threads)
    except KeyboardInterrupt:
        # Handle keyboard interrupt (Ctrl+C)
        print("\nQuitting...")  # Print message before quitting
        sys.exit(0)  # Exit gracefully

if __name__ == '__main__':
    main()
