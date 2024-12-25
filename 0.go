package main

import (
	"fmt"
	"time"

	"github.com/bwmarrin/discordgo"
)

func main() {
	Token := "YOUR_BOT_TOKEN" // Replace with your Discord bot token

	dg, err := discordgo.New("Bot " + Token)
	if err != nil {
		fmt.Println("error creating Discord session,", err)
		return
	}

	dg.AddMessageCreateHandler(messageCreate)

	err = dg.Open()
	if err != nil {
		fmt.Println("error opening connection,", err)
		return
	}
	defer dg.Close()

	fmt.Println("Bot is now running. Press CTRL+C to exit.")
	select {}
}

func messageCreate(s *discordgo.Session, m *discordgo.MessageCreate) {
	// Ignore messages from the bot itself
	if m.Author.ID == s.State.User.ID {
		return
	}

	// Check if the message is "!spam" command
	if m.Content == "!spam" {
		// Start spamming "test" 500 times in 5 seconds
		for i := 0; i < 500; i++ {
			// Simulate a delay to spread out the spam
			time.Sleep(10 * time.Millisecond)
			s.ChannelMessageSend(m.ChannelID, "test")
		}
	}
}
