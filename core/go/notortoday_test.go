package notortoday

import (
	"os"
	"testing"
)

const sampleData = `ExitAddress 185.220.101.1 2024-01-01 12:00:00
ExitAddress 185.220.101.2 2024-01-01 12:00:00
ExitAddress 185.220.101.3 2024-01-01 12:00:00
ExitAddress 185.220.102.1 2024-01-01 12:00:00
ExitAddress 185.220.102.2 2024-01-01 12:00:00
ExitAddress 95.214.55.43 2024-01-01 12:00:00
ExitAddress 95.214.55.44 2024-01-01 12:00:00
ExitAddress 162.247.74.74 2024-01-01 12:00:00
ExitAddress 162.247.74.75 2024-01-01 12:00:00
ExitAddress 176.10.99.200 2024-01-01 12:00:00`

func TestParseExitAddresses(t *testing.T) {
	nt, err := New(false, "")
	if err != nil {
		t.Fatalf("Failed to create NoTorToday: %v", err)
	}

	nt.ParseExitAddresses(sampleData)

	if nt.GetCount() != 10 {
		t.Errorf("Expected 10 exit nodes, got %d", nt.GetCount())
	}
}

func TestIdentifyTorIPs(t *testing.T) {
	nt, err := New(false, "")
	if err != nil {
		t.Fatalf("Failed to create NoTorToday: %v", err)
	}

	nt.ParseExitAddresses(sampleData)

	testCases := []struct {
		ip       string
		expected bool
		desc     string
	}{
		{"185.220.101.1", true, "185.220.101.1 should be Tor"},
		{"185.220.101.2", true, "185.220.101.2 should be Tor"},
		{"8.8.8.8", false, "8.8.8.8 should not be Tor"},
		{"1.1.1.1", false, "1.1.1.1 should not be Tor"},
	}

	for _, tc := range testCases {
		result := nt.IsTor(tc.ip)
		if result != tc.expected {
			t.Errorf("%s: got %v, want %v", tc.desc, result, tc.expected)
		}
	}
}

func TestValidateIPv4(t *testing.T) {
	nt, err := New(false, "")
	if err != nil {
		t.Fatalf("Failed to create NoTorToday: %v", err)
	}

	testCases := []struct {
		ip       string
		expected bool
	}{
		{"192.168.1.1", true},
		{"255.255.255.255", true},
		{"0.0.0.0", true},
		{"256.1.1.1", false},
		{"invalid", false},
		{"192.168.1", false},
	}

	for _, tc := range testCases {
		result := nt.isValidIP(tc.ip)
		if result != tc.expected {
			t.Errorf("IP %s: got %v, want %v", tc.ip, result, tc.expected)
		}
	}
}

func TestValidateIPv6(t *testing.T) {
	nt, err := New(false, "")
	if err != nil {
		t.Fatalf("Failed to create NoTorToday: %v", err)
	}

	testCases := []struct {
		ip       string
		expected bool
	}{
		{"2001:0db8:85a3:0000:0000:8a2e:0370:7334", true},
		{"::1", true},
		{"invalid:ipv6", false},
	}

	for _, tc := range testCases {
		result := nt.isValidIP(tc.ip)
		if result != tc.expected {
			t.Errorf("IP %s: got %v, want %v", tc.ip, result, tc.expected)
		}
	}
}

func TestCacheOperations(t *testing.T) {
	nt, err := New(false, "")
	if err != nil {
		t.Fatalf("Failed to create NoTorToday: %v", err)
	}

	nt.ParseExitAddresses(sampleData)
	nt.saveCache(sampleData)

	// Create new instance and load cache
	nt2, err := New(false, "")
	if err != nil {
		t.Fatalf("Failed to create second NoTorToday: %v", err)
	}

	loaded := nt2.loadCache()
	if !loaded {
		t.Error("Should load cache successfully")
	}

	if nt2.GetCount() != 10 {
		t.Errorf("Expected 10 exit nodes after loading cache, got %d", nt2.GetCount())
	}

	// Cleanup
	_ = os.WriteFile(nt.cacheFile, []byte(""), 0644)
}
