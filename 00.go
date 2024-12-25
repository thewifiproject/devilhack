package main

import (
	"fmt"
	"log"
	"strings"
	"time"

	"github.com/bwmarrin/discordgo"
)

// Replace this with your bot token
const token = ""

// List of offensive words (add real words here)
var offensiveWords = []string{
	"hajzl", "černochu", "znásilnit", "penis", "grázle", "hajzle", "dick", "negr", "kokot", "píčo", // Add your own list here
}

// List of forbidden websites
var forbiddenLinks = []string{
	"pornhub.com", "xnxx.com", "fr.pornhub.com", "esigfau.com",
}

// Keeps track of user activity for anti-spam
var userMessages = make(map[string][]time.Time)

// Tracks recent member joins for anti-raid
var recentJoins = make([]time.Time, 0)

// Check if a message contains offensive words
func containsOffensive(message string) bool {
	for _, word := range offensiveWords {
		if strings.Contains(strings.ToLower(message), word) {
			return true
		}
	}
	return false
}

// Check if a message contains forbidden links
func containsForbiddenLink(message string) bool {
	for _, link := range forbiddenLinks {
		if strings.Contains(strings.ToLower(message), link) {
			return true
		}
	}
	return false
}

// Check if the user is a bot
func isBot(userID string) bool {
	// You can modify this check to verify if the user is a bot.
	// Discord provides the "bot" property on user objects to easily check.
	return strings.HasPrefix(userID, "bot") // Simplified check, replace with actual check if needed.
}

// Handle messages
func messageCreate(s *discordgo.Session, m *discordgo.MessageCreate) {
	if m.Author.ID == s.State.User.ID {
		// Ignore messages sent by the bot itself
		return
	}

	// Anti-Spam Check: Record the time of the message
	if _, exists := userMessages[m.Author.ID]; !exists {
		userMessages[m.Author.ID] = []time.Time{}
	}

	userMessages[m.Author.ID] = append(userMessages[m.Author.ID], time.Now())

	// Remove old messages (keep only messages within the last 2 seconds)
	var recentMessages []time.Time
	for _, msgTime := range userMessages[m.Author.ID] {
		if time.Since(msgTime) < 2*time.Second { // Changed to 2 seconds for spam detection
			recentMessages = append(recentMessages, msgTime)
		}
	}
	userMessages[m.Author.ID] = recentMessages

	// If more than 13 messages within 2 seconds, kick user
	if len(recentMessages) >= 13 {
		s.ChannelMessageDelete(m.ChannelID, m.ID)

		// Check if the user is a bot and kick if banning failed
		if isBot(m.Author.ID) {
			err := s.GuildMemberDelete(m.GuildID, m.Author.ID) // Kick bot if banning fails
			if err != nil {
				log.Printf("Error kicking bot: %v", err)
			} else {
				s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("%s (bot) has been kicked for spamming.", m.Author.Mention()))
			}
		} else {
			err := s.GuildMemberDelete(m.GuildID, m.Author.ID) // Kick regular user
			if err != nil {
				log.Printf("Error kicking user: %v", err)
			} else {
				s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("%s has been kicked for spamming.", m.Author.Mention()))
			}
		}
		return
	}

	// Offensive word detection
	if containsOffensive(m.Content) {
		s.ChannelMessageDelete(m.ChannelID, m.ID)
		s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("%s used offensive language and was timed out.", m.Author.Mention()))

		// Apply timeout logic: Track user offenses
		timeoutTime := time.Now().Add(9 * time.Minute)
		err := s.GuildMemberTimeout(m.GuildID, m.Author.ID, &timeoutTime) // Pass pointer to time
		if err != nil {
			log.Printf("Error applying timeout: %v", err)
		}
		return
	}

	// Forbidden link detection
	if containsForbiddenLink(m.Content) {
		s.ChannelMessageDelete(m.ChannelID, m.ID)
		s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("%s posted a forbidden link and was banned.", m.Author.Mention()))

		// Ban the user for posting a forbidden link (0 days of deleted messages)
		err := s.GuildBanCreate(m.GuildID, m.Author.ID, 0) // Correct method for banning with no deleted messages
		if err != nil {
			// If banning fails, try kicking
			err := s.GuildMemberDelete(m.GuildID, m.Author.ID) // Kick instead of banning
			if err != nil {
				log.Printf("Error kicking user: %v", err)
			} else {
				s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("%s was kicked for posting a forbidden link.", m.Author.Mention()))
			}
		}
	}
}

// Handle member join (anti-raid protection)
func memberJoin(s *discordgo.Session, m *discordgo.GuildMemberAdd) {
	// Record the join time of the new member
	recentJoins = append(recentJoins, time.Now())

	// Clean up the join records older than 10 seconds
	var recentJoinsFiltered []time.Time
	for _, joinTime := range recentJoins {
		if time.Since(joinTime) < 10*time.Second { // 10 seconds window for detecting raid
			recentJoinsFiltered = append(recentJoinsFiltered, joinTime)
		}
	}
	recentJoins = recentJoinsFiltered

	// If more than 5 new members join within 10 seconds, assume it's a raid
	if len(recentJoins) > 5 {
		// Ban all members who joined recently (preventing raid members from causing chaos)
		// We only ban the latest join, assuming it's a raid
		for _, joinTime := range recentJoins {
			if isBot(m.User.ID) {
				// Try banning the bot if it's detected in a raid
				err := s.GuildBanCreate(m.GuildID, m.User.ID, 0) // Ban without deleting messages
				if err != nil {
					// If banning fails, kick the bot instead
					err := s.GuildMemberDelete(m.GuildID, m.User.ID) // Kick bot if banning fails
					if err != nil {
						log.Printf("Error kicking bot: %v", err)
					} else {
						s.ChannelMessageSend(m.GuildID, fmt.Sprintf("%s (bot) was kicked for suspected raid activity.", m.User.Mention()))
					}
				} else {
					s.ChannelMessageSend(m.GuildID, fmt.Sprintf("%s (bot) was banned for suspected raid activity.", m.User.Mention()))
				}
			} else {
				// Handle non-bot member banning
				err := s.GuildBanCreate(m.GuildID, m.User.ID, 0) // Ban without deleting messages
				if err != nil {
					log.Printf("Error banning user: %v", err)
				} else {
					s.ChannelMessageSend(m.GuildID, fmt.Sprintf("%s was banned for suspected raid activity.", m.User.Mention()))
				}
			}
		}
		return
	}
}

// Handle /spamban command
func commandHandler(s *discordgo.Session, m *discordgo.MessageCreate) {
	if m.Author.ID == s.State.User.ID {
		return
	}

	// Check if the message is the /spamban command
	if strings.HasPrefix(m.Content, "/spamban") {
		// Only allow admins to use this command
		member, err := s.GuildMember(m.GuildID, m.Author.ID)
		if err != nil {
			log.Println("Error fetching member:", err)
			return
		}

		// Check if the user is an administrator
		isAdmin := false
		for _, role := range member.Roles {
			roleObj, err := s.State.Role(m.GuildID, role)
			if err != nil {
				log.Println("Error fetching role:", err)
				continue
			}
			if roleObj.Permissions&discordgo.PermissionAdministrator != 0 {
				isAdmin = true
				break
			}
		}

		if !isAdmin {
			s.ChannelMessageSend(m.ChannelID, "You must be an admin to use this command.")
			return
		}

		// Get the user mentioned in the command
		args := strings.Fields(m.Content)
		if len(args) < 2 {
			s.ChannelMessageSend(m.ChannelID, "Please mention a user to ban (e.g. `/spamban @user`).")
			return
		}

		// Mentioned user
		userID := strings.TrimPrefix(args[1], "<@!")
		userID = strings.TrimSuffix(userID, ">")

		// Ban the user
		err = s.GuildBanCreate(m.GuildID, userID, 0) // Ban without deleting messages
		if err != nil {
			log.Printf("Error banning user: %v", err)
			s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("Failed to ban the user: %v", err))
			return
		}

		s.ChannelMessageSend(m.ChannelID, fmt.Sprintf("<@%s> has been banned for suspected spamming.", userID))
	}
}

// Bot initialization
func main() {
	// Create a new Discord session using the provided bot token
	dg, err := discordgo.New("Bot " + token)
	if err != nil {
		log.Fatalf("error creating Discord session: %v", err)
		return
	}

	// Register messageCreate as the callback for MessageCreate events
	dg.AddHandler(messageCreate)

	// Register memberJoin as the callback for MemberAdd events (for anti-raid protection)
	dg.AddHandler(memberJoin)

	// Register commandHandler as the callback for messageCreate events (for /spamban command)
	dg.AddHandler(commandHandler)

	// Open a websocket connection to Discord and begin listening
	err = dg.Open()
	if err != nil {
		log.Fatalf("error opening connection: %v", err)
		return
	}

	fmt.Println("Bot is now running. Press Ctrl+C to exit.")
	select {} // Keeps the bot running
}
