package main

import (
	"fmt"
	"log"
	
	"github.com/TheusHen/NoTorToday/core/go"
)

func main() {
	fmt.Println("NoTorToday - Go Example\n")

	// Create instance with auto-update
	checker, err := notortoday.New(true, "")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("Loaded %d Tor exit nodes\n", checker.GetCount())
	if lastUpdate := checker.GetLastUpdate(); lastUpdate != nil {
		fmt.Printf("Last update: %s\n\n", lastUpdate)
	}

	// Test IPs
	testIPs := []string{
		"185.220.101.1", // Example Tor IP (may or may not be in list)
		"8.8.8.8",       // Google DNS (not Tor)
		"1.1.1.1",       // Cloudflare DNS (not Tor)
	}

	fmt.Println("Testing IP addresses:")
	for _, ip := range testIPs {
		isTor := checker.IsTor(ip)
		status := "Not Tor"
		if isTor {
			status = "TOR EXIT NODE"
		}
		fmt.Printf("  %s: %s\n", ip, status)
	}
}
