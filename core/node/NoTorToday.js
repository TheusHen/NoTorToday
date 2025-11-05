const fs = require('fs');
const https = require('https');
const path = require('path');
const os = require('os');

/**
 * NoTorToday - Check if an IP address belongs to a Tor exit node
 */
class NoTorToday {
  constructor(autoUpdate = true, cacheDir = null) {
    this.data = new Set();
    this.lastUpdate = null;
    this.cacheDir = cacheDir || process.cwd();
    this.cacheFile = path.join(this.cacheDir, '.notortoday_cache');
    this.torListUrl = 'https://check.torproject.org/exit-addresses';
    
    if (autoUpdate) {
      this.updateList();
    } else {
      this.loadCache();
    }
  }

  /**
   * Download and parse the Tor exit node list
   */
  async updateList() {
    try {
      const data = await this.downloadTorList();
      this.parseExitAddresses(data);
      this.saveCache(data);
      this.lastUpdate = new Date();
      return true;
    } catch (error) {
      // If download fails, try to load from cache
      if (this.loadCache()) {
        return true;
      }
      throw error;
    }
  }

  /**
   * Download Tor exit node list from torproject.org
   */
  downloadTorList() {
    return new Promise((resolve, reject) => {
      https.get(this.torListUrl, (res) => {
        if (res.statusCode !== 200) {
          reject(new Error(`HTTP ${res.statusCode}`));
          return;
        }
        
        let data = '';
        res.on('data', (chunk) => { data += chunk; });
        res.on('end', () => resolve(data));
      }).on('error', reject);
    });
  }

  /**
   * Parse exit addresses from the downloaded data
   */
  parseExitAddresses(data) {
    this.data.clear();
    const lines = data.split('\n');
    
    for (const line of lines) {
      if (line.startsWith('ExitAddress ')) {
        const parts = line.split(' ');
        if (parts.length >= 2) {
          const ip = parts[1].trim();
          if (this.isValidIP(ip)) {
            this.data.add(ip);
          }
        }
      }
    }
  }

  /**
   * Validate IP address (IPv4 and IPv6)
   */
  isValidIP(ip) {
    // IPv4 validation
    const ipv4Regex = /^(\d{1,3}\.){3}\d{1,3}$/;
    if (ipv4Regex.test(ip)) {
      const parts = ip.split('.');
      return parts.every(part => parseInt(part) >= 0 && parseInt(part) <= 255);
    }
    
    // IPv6 validation (basic)
    const ipv6Regex = /^([0-9a-fA-F]{0,4}:){2,7}[0-9a-fA-F]{0,4}$/;
    return ipv6Regex.test(ip);
  }

  /**
   * Check if an IP address is a Tor exit node
   */
  isTor(ip) {
    return this.data.has(ip);
  }

  /**
   * Get the number of loaded Tor exit nodes
   */
  getCount() {
    return this.data.size;
  }

  /**
   * Get the last update timestamp
   */
  getLastUpdate() {
    return this.lastUpdate;
  }

  /**
   * Save cache to disk
   */
  saveCache(data) {
    try {
      fs.writeFileSync(this.cacheFile, data, 'utf8');
    } catch (error) {
      // Silently fail if cache can't be written
    }
  }

  /**
   * Load cache from disk
   */
  loadCache() {
    try {
      if (fs.existsSync(this.cacheFile)) {
        const data = fs.readFileSync(this.cacheFile, 'utf8');
        this.parseExitAddresses(data);
        const stats = fs.statSync(this.cacheFile);
        this.lastUpdate = stats.mtime;
        return true;
      }
    } catch (error) {
      // Silently fail if cache can't be read
    }
    return false;
  }
}

module.exports = NoTorToday;
