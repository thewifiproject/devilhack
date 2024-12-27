import socket
import argparse

def start_listener(lhost, lport):
    # Create a socket object
    listener_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Bind the socket to the host and port
    listener_socket.bind((lhost, lport))
    
    # Start listening for incoming connections (backlog set to 5)
    listener_socket.listen(5)
    print(f"Listening on {lhost}:{lport}...")
    
    try:
        # Accept incoming connection
        client_socket, client_address = listener_socket.accept()
        print(f"Connection received from {client_address}")
        
        # Handle communication with the client
        while True:
            # Receive data from the client
            data = client_socket.recv(1024)  # Buffer size is 1024 bytes
            if not data:
                print("Connection closed by client.")
                break

            # Print received data and send back a response
            print(f"Received: {data.decode()}")
            client_socket.sendall(data)  # Echo back the received data

    except KeyboardInterrupt:
        print("\nListener interrupted.")
    finally:
        # Close the sockets when done
        listener_socket.close()
        print("Listener closed.")

def main():
    # Initialize the argument parser
    parser = argparse.ArgumentParser(description="Simple Python listener (like Netcat)")
    parser.add_argument("LHOST", type=str, help="Local host IP address to listen on")
    parser.add_argument("LPORT", type=int, help="Local port number to listen on")
    
    # Parse arguments
    args = parser.parse_args()
    
    # Start the listener
    start_listener(args.LHOST, args.LPORT)

if __name__ == "__main__":
    main()
