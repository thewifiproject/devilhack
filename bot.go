package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"mime/multipart"
	"net/http"
	"os"
	"regexp"
	"strings"

	"github.com/bwmarrin/discordgo"
)

const token = "YOUR_BOT_TOKEN"
const virusTotalAPIKey = "YOUR_VIRUSTOTAL_API_KEY"

// Forbidden words list
var forbiddenWords = []string{
	"kokot", "píča", "kundčka", "grázl", "!nuke", "nude", "úchyl", "debil",
	"penis", "nadržený", "šukat", "černoch", "negr",
}

// Function to check for forbidden words
func containsForbiddenWords(message string) bool {
	lowerMessage := strings.ToLower(message)
	for _, word := range forbiddenWords {
		if strings.Contains(lowerMessage, word) {
			return true
		}
	}
	return false
}

// Function to scan URLs with VirusTotal
func scanURLWithVirusTotal(url string) (bool, error) {
	apiURL := "https://www.virustotal.com/vtapi/v2/url/scan"
	data := map[string]string{"url": url, "apikey": virusTotalAPIKey}
	jsonData, _ := json.Marshal(data)

	resp, err := http.Post(apiURL, "application/json", bytes.NewBuffer(jsonData))
	if err != nil {
		return false, fmt.Errorf("failed to send URL to VirusTotal: %w", err)
	}
	defer resp.Body.Close()

	body, _ := io.ReadAll(resp.Body)
	fmt.Printf("VirusTotal API Response (URL Scan): %s\n", body)

	if resp.StatusCode != http.StatusOK {
		return false, fmt.Errorf("VirusTotal responded with status code: %d", resp.StatusCode)
	}

	var result map[string]interface{}
	if err := json.Unmarshal(body, &result); err != nil {
		return false, fmt.Errorf("failed to decode VirusTotal response: %w", err)
	}

	if scanID, ok := result["scan_id"].(string); ok {
		return checkURLScanResult(scanID)
	}

	return false, fmt.Errorf("scan_id not found in VirusTotal response")
}

// Function to check URL scan results with VirusTotal
func checkURLScanResult(scanID string) (bool, error) {
	apiURL := fmt.Sprintf("https://www.virustotal.com/vtapi/v2/url/report?apikey=%s&resource=%s", virusTotalAPIKey, scanID)

	resp, err := http.Get(apiURL)
	if err != nil {
		return false, fmt.Errorf("failed to query VirusTotal: %w", err)
	}
	defer resp.Body.Close()

	body, _ := io.ReadAll(resp.Body)
	fmt.Printf("VirusTotal API Response (URL Report): %s\n", body)

	if resp.StatusCode != http.StatusOK {
		return false, fmt.Errorf("VirusTotal responded with status code: %d", resp.StatusCode)
	}

	var result map[string]interface{}
	if err := json.Unmarshal(body, &result); err != nil {
		return false, fmt.Errorf("failed to decode VirusTotal response: %w", err)
	}

	if positives, ok := result["positives"].(float64); ok && positives > 0 {
		return true, nil
	}

	return false, nil
}

// Function to handle messages
func messageCreate(s *discordgo.Session, m *discordgo.MessageCreate) {
	if m.Author.ID == s.State.User.ID {
		return
	}

	// Check for forbidden words
	if containsForbiddenWords(m.Content) {
		s.ChannelMessageDelete(m.ChannelID, m.ID)
		s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("%s, your message was deleted for containing forbidden words.", m.Author.Mention()))
		return
	}

	// Check for URLs in the message
	re := regexp.MustCompile(`https?://[^\s]+`)
	urls := re.FindAllString(m.Content, -1)
	for _, url := range urls {
		message, _ := s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("%s Scanning content...", m.Author.Mention()))

		isMalicious, err := scanURLWithVirusTotal(url)
		if err != nil {
			s.ChannelMessageEdit(m.ChannelID, message.ID, fmt.Sprintf("%s, an error occurred during URL scanning: %v", m.Author.Mention(), err))
			continue
		}

		if isMalicious {
			s.ChannelMessageDelete(m.ChannelID, m.ID)
			s.ChannelMessageEdit(m.ChannelID, message.ID, fmt.Sprintf("%s, VIRUS DETECTED! Your URL was deleted.", m.Author.Mention()))
			return
		}

		s.ChannelMessageEdit(m.ChannelID, message.ID, fmt.Sprintf("No virus detected in URL: %s", url))
	}
}

func main() {
	dg, err := discordgo.New("Bot " + token)
	if err != nil {
		fmt.Printf("Error creating Discord session: %v\n", err)
		return
	}

	dg.AddHandler(messageCreate)

	err = dg.Open()
	if err != nil {
		fmt.Printf("Error opening Discord session: %v\n", err)
		return
	}

	fmt.Println("Bot is now running. Press Ctrl+C to exit.")
	select {}
}
