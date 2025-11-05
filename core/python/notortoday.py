import os
import re
import urllib.request
from datetime import datetime
from typing import Set, Optional

class NoTorToday:
    """Check if an IP address belongs to a Tor exit node"""
    
    def __init__(self, auto_update: bool = True, cache_dir: Optional[str] = None):
        self.data: Set[str] = set()
        self.last_update: Optional[datetime] = None
        self.cache_dir = cache_dir or os.getcwd()
        self.cache_file = os.path.join(self.cache_dir, '.notortoday_cache')
        self.tor_list_url = 'https://check.torproject.org/exit-addresses'
        
        if auto_update:
            self.update_list()
        else:
            self.load_cache()
    
    def update_list(self) -> bool:
        """Download and parse the Tor exit node list"""
        try:
            data = self.download_tor_list()
            self.parse_exit_addresses(data)
            self.save_cache(data)
            self.last_update = datetime.now()
            return True
        except Exception:
            # If download fails, try to load from cache
            if self.load_cache():
                return True
            raise
    
    def download_tor_list(self) -> str:
        """Download Tor exit node list from torproject.org"""
        with urllib.request.urlopen(self.tor_list_url) as response:
            return response.read().decode('utf-8')
    
    def parse_exit_addresses(self, data: str) -> None:
        """Parse exit addresses from the downloaded data"""
        self.data.clear()
        lines = data.split('\n')
        
        for line in lines:
            if line.startswith('ExitAddress '):
                parts = line.split()
                if len(parts) >= 2:
                    ip = parts[1].strip()
                    if self.is_valid_ip(ip):
                        self.data.add(ip)
    
    def is_valid_ip(self, ip: str) -> bool:
        """Validate IP address (IPv4 and IPv6)"""
        # IPv4 validation
        ipv4_pattern = r'^(\d{1,3}\.){3}\d{1,3}$'
        if re.match(ipv4_pattern, ip):
            parts = ip.split('.')
            return all(0 <= int(part) <= 255 for part in parts)
        
        # IPv6 validation (basic)
        ipv6_pattern = r'^([0-9a-fA-F]{0,4}:){2,7}[0-9a-fA-F]{0,4}$'
        return bool(re.match(ipv6_pattern, ip))
    
    def is_tor(self, ip: str) -> bool:
        """Check if an IP address is a Tor exit node"""
        return ip in self.data
    
    def get_count(self) -> int:
        """Get the number of loaded Tor exit nodes"""
        return len(self.data)
    
    def get_last_update(self) -> Optional[datetime]:
        """Get the last update timestamp"""
        return self.last_update
    
    def save_cache(self, data: str) -> None:
        """Save cache to disk"""
        try:
            with open(self.cache_file, 'w', encoding='utf-8') as f:
                f.write(data)
        except Exception:
            # Silently fail if cache can't be written
            pass
    
    def load_cache(self) -> bool:
        """Load cache from disk"""
        try:
            if os.path.exists(self.cache_file):
                with open(self.cache_file, 'r', encoding='utf-8') as f:
                    data = f.read()
                self.parse_exit_addresses(data)
                stat = os.stat(self.cache_file)
                self.last_update = datetime.fromtimestamp(stat.st_mtime)
                return True
        except Exception:
            # Silently fail if cache can't be read
            pass
        return False
