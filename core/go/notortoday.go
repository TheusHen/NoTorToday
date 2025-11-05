package notortoday

import (
	"bufio"
	"io"
	"net"
	"net/http"
	"os"
	"path/filepath"
	"strings"
	"time"
)

// NoTorToday checks if an IP address belongs to a Tor exit node
type NoTorToday struct {
	data       map[string]bool
	lastUpdate *time.Time
	cacheFile  string
	torListURL string
}

// New creates a new NoTorToday instance
func New(autoUpdate bool, cacheDir string) (*NoTorToday, error) {
	if cacheDir == "" {
		var err error
		cacheDir, err = os.Getwd()
		if err != nil {
			return nil, err
		}
	}

	nt := &NoTorToday{
		data:       make(map[string]bool),
		cacheFile:  filepath.Join(cacheDir, ".notortoday_cache"),
		torListURL: "https://check.torproject.org/exit-addresses",
	}

	if autoUpdate {
		if err := nt.UpdateList(); err != nil {
			// Try to load from cache if update fails
			if !nt.loadCache() {
				return nil, err
			}
		}
	} else {
		nt.loadCache()
	}

	return nt, nil
}

// UpdateList downloads and parses the Tor exit node list
func (nt *NoTorToday) UpdateList() error {
	data, err := nt.downloadTorList()
	if err != nil {
		return err
	}

	nt.ParseExitAddresses(data)
	nt.saveCache(data)
	now := time.Now()
	nt.lastUpdate = &now

	return nil
}

// downloadTorList downloads Tor exit node list from torproject.org
func (nt *NoTorToday) downloadTorList() (string, error) {
	resp, err := http.Get(nt.torListURL)
	if err != nil {
		return "", err
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	return string(body), nil
}

// ParseExitAddresses parses exit addresses from the downloaded data (exported for testing)
func (nt *NoTorToday) ParseExitAddresses(data string) {
	nt.data = make(map[string]bool)
	scanner := bufio.NewScanner(strings.NewReader(data))

	for scanner.Scan() {
		line := scanner.Text()
		if strings.HasPrefix(line, "ExitAddress ") {
			parts := strings.Fields(line)
			if len(parts) >= 2 {
				ip := strings.TrimSpace(parts[1])
				if nt.isValidIP(ip) {
					nt.data[ip] = true
				}
			}
		}
	}
}

// isValidIP validates IP address (IPv4 and IPv6)
func (nt *NoTorToday) isValidIP(ip string) bool {
	return net.ParseIP(ip) != nil
}

// IsTor checks if an IP address is a Tor exit node
func (nt *NoTorToday) IsTor(ip string) bool {
	return nt.data[ip]
}

// GetCount returns the number of loaded Tor exit nodes
func (nt *NoTorToday) GetCount() int {
	return len(nt.data)
}

// GetLastUpdate returns the last update timestamp
func (nt *NoTorToday) GetLastUpdate() *time.Time {
	return nt.lastUpdate
}

// saveCache saves cache to disk
func (nt *NoTorToday) saveCache(data string) {
	os.WriteFile(nt.cacheFile, []byte(data), 0644)
}

// loadCache loads cache from disk
func (nt *NoTorToday) loadCache() bool {
	if _, err := os.Stat(nt.cacheFile); os.IsNotExist(err) {
		return false
	}

	data, err := os.ReadFile(nt.cacheFile)
	if err != nil {
		return false
	}

	nt.ParseExitAddresses(string(data))

	info, err := os.Stat(nt.cacheFile)
	if err == nil {
		modTime := info.ModTime()
		nt.lastUpdate = &modTime
	}

	return true
}
