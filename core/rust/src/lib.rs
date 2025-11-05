use std::collections::HashSet;
use std::fs;
use std::net::IpAddr;
use std::path::PathBuf;
use std::time::SystemTime;

/// NoTorToday checks if an IP address belongs to a Tor exit node
pub struct NoTorToday {
    data: HashSet<String>,
    last_update: Option<SystemTime>,
    cache_file: PathBuf,
    tor_list_url: String,
}

impl NoTorToday {
    /// Create a new NoTorToday instance
    pub fn new(auto_update: bool, cache_dir: Option<String>) -> Result<Self, Box<dyn std::error::Error>> {
        let cache_dir = cache_dir.unwrap_or_else(|| {
            std::env::current_dir()
                .unwrap()
                .to_str()
                .unwrap()
                .to_string()
        });

        let cache_file = PathBuf::from(cache_dir).join(".notortoday_cache");

        let mut nt = NoTorToday {
            data: HashSet::new(),
            last_update: None,
            cache_file,
            tor_list_url: "https://check.torproject.org/exit-addresses".to_string(),
        };

        if auto_update {
            if let Err(_) = nt.update_list() {
                // Try to load from cache if update fails
                if !nt.load_cache() {
                    return Err("Failed to update and no cache available".into());
                }
            }
        } else {
            nt.load_cache();
        }

        Ok(nt)
    }

    /// Download and parse the Tor exit node list
    pub fn update_list(&mut self) -> Result<(), Box<dyn std::error::Error>> {
        let data = self.download_tor_list()?;
        self.parse_exit_addresses(&data);
        self.save_cache(&data);
        self.last_update = Some(SystemTime::now());
        Ok(())
    }

    /// Download Tor exit node list from torproject.org
    fn download_tor_list(&self) -> Result<String, Box<dyn std::error::Error>> {
        let response = reqwest::blocking::get(&self.tor_list_url)?;
        let body = response.text()?;
        Ok(body)
    }

    /// Parse exit addresses from the downloaded data
    fn parse_exit_addresses(&mut self, data: &str) {
        self.data.clear();

        for line in data.lines() {
            if line.starts_with("ExitAddress ") {
                let parts: Vec<&str> = line.split_whitespace().collect();
                if parts.len() >= 2 {
                    let ip = parts[1].trim();
                    if self.is_valid_ip(ip) {
                        self.data.insert(ip.to_string());
                    }
                }
            }
        }
    }

    /// Validate IP address (IPv4 and IPv6)
    fn is_valid_ip(&self, ip: &str) -> bool {
        ip.parse::<IpAddr>().is_ok()
    }

    /// Check if an IP address is a Tor exit node
    pub fn is_tor(&self, ip: &str) -> bool {
        self.data.contains(ip)
    }

    /// Get the number of loaded Tor exit nodes
    pub fn get_count(&self) -> usize {
        self.data.len()
    }

    /// Get the last update timestamp
    pub fn get_last_update(&self) -> Option<SystemTime> {
        self.last_update
    }

    /// Save cache to disk
    fn save_cache(&self, data: &str) {
        let _ = fs::write(&self.cache_file, data);
    }

    /// Load cache from disk
    fn load_cache(&mut self) -> bool {
        if let Ok(data) = fs::read_to_string(&self.cache_file) {
            self.parse_exit_addresses(&data);

            if let Ok(metadata) = fs::metadata(&self.cache_file) {
                if let Ok(modified) = metadata.modified() {
                    self.last_update = Some(modified);
                }
            }

            return true;
        }
        false
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_is_valid_ip() {
        let nt = NoTorToday {
            data: HashSet::new(),
            last_update: None,
            cache_file: PathBuf::from(".notortoday_cache"),
            tor_list_url: String::new(),
        };

        assert!(nt.is_valid_ip("192.168.1.1"));
        assert!(nt.is_valid_ip("2001:0db8:85a3:0000:0000:8a2e:0370:7334"));
        assert!(!nt.is_valid_ip("invalid"));
    }
}
