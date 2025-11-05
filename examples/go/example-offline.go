package main

import (
	"fmt"
	"log"
	"os"
	"path/filepath"
	
	"github.com/TheusHen/NoTorToday/core/go"
)

func main() {
	fmt.Println("NoTorToday - Go Example (Offline Mode)\n")

	// Create instance without auto-update
	checker, err := notortoday.New(false, "")
	if err != nil {
		log.Fatal(err)
	}

	// Load sample data for testing
	samplePath := filepath.Join("../../datasets/sample-exit-addresses.txt")
	data, err := os.ReadFile(samplePath)
	if err != nil {
		log.Fatal(err)
	}
	
	checker.ParseExitAddresses(string(data))

	fmt.Printf("Loaded %d Tor exit nodes\n", checker.GetCount())
	fmt.Println("Using sample dataset for offline testing\n")

	// Test IPs
	testIPs := []string{
		"185.220.101.1", // In sample dataset - should be Tor
		"185.220.101.2", // In sample dataset - should be Tor
		"8.8.8.8",       // Google DNS - not Tor
		"1.1.1.1",       // Cloudflare DNS - not Tor
	}

	fmt.Println("Testing IP addresses:")
	for _, ip := range testIPs {
		isTor := checker.IsTor(ip)
		status := "Not Tor ✗"
		if isTor {
			status = "TOR EXIT NODE ✓"
		}
		fmt.Printf("  %s: %s\n", ip, status)
	}
}
