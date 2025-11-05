package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"time"

	"github.com/TheusHen/NoTorToday/core/go"
)

func main() {
	iterations := 1000

	// Initialize
	checker, err := notortoday.New(false, "")
	if err != nil {
		panic(err)
	}

	// Load sample data for offline testing
	samplePath := filepath.Join("../../datasets/sample-exit-addresses.txt")
	data, err := ioutil.ReadFile(samplePath)
	if err != nil {
		// Try to update if sample not available
		if err := checker.UpdateList(); err != nil {
			fmt.Fprintf(os.Stderr, "Cannot load data: %v\n", err)
			os.Exit(1)
		}
	} else {
		checker.ParseExitAddresses(string(data))
	}

	testIPs := []string{
		"185.220.101.1",
		"8.8.8.8",
		"1.1.1.1",
		"192.168.1.1",
		"10.0.0.1",
	}

	// Warmup
	for i := 0; i < 100; i++ {
		for _, ip := range testIPs {
			checker.IsTor(ip)
		}
	}

	// Benchmark
	start := time.Now()

	for i := 0; i < iterations; i++ {
		for _, ip := range testIPs {
			checker.IsTor(ip)
		}
	}

	duration := time.Since(start)
	durationMs := float64(duration.Nanoseconds()) / 1000000.0
	opsPerSec := int(float64(iterations*len(testIPs)) / (durationMs / 1000.0))
	
	// Handle edge case where duration is too small
	if durationMs < 0.01 {
		durationMs = 0.01
		opsPerSec = iterations * len(testIPs) * 100
	}

	result := map[string]interface{}{
		"language":           "Go",
		"duration_ms":        fmt.Sprintf("%.2f", durationMs),
		"operations":         iterations * len(testIPs),
		"ops_per_second":     opsPerSec,
		"exit_nodes_loaded":  checker.GetCount(),
	}

	jsonData, _ := json.Marshal(result)
	fmt.Println(string(jsonData))
}
