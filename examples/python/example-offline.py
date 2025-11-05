import sys
import os

# Add core directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../core/python'))

from notortoday import NoTorToday

def main():
    print('NoTorToday - Python Example (Offline Mode)\n')

    # Create instance without auto-update
    checker = NoTorToday(auto_update=False)
    
    # Load sample data for testing
    sample_path = os.path.join(os.path.dirname(__file__), 
                               '../../datasets/sample-exit-addresses.txt')
    with open(sample_path, 'r') as f:
        sample_data = f.read()
    checker.parse_exit_addresses(sample_data)
    
    print(f'Loaded {checker.get_count()} Tor exit nodes')
    print('Using sample dataset for offline testing\n')

    # Test IPs
    test_ips = [
        '185.220.101.1',  # In sample dataset - should be Tor
        '185.220.101.2',  # In sample dataset - should be Tor
        '8.8.8.8',        # Google DNS - not Tor
        '1.1.1.1',        # Cloudflare DNS - not Tor
    ]

    print('Testing IP addresses:')
    for ip in test_ips:
        is_tor = checker.is_tor(ip)
        status = 'TOR EXIT NODE ✓' if is_tor else 'Not Tor ✗'
        print(f'  {ip}: {status}')

if __name__ == '__main__':
    main()
