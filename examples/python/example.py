import sys
import os

# Add core directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../core/python'))

from notortoday import NoTorToday

def main():
    print('NoTorToday - Python Example\n')

    # Create instance with auto-update
    checker = NoTorToday(auto_update=True)
    
    print(f'Loaded {checker.get_count()} Tor exit nodes')
    print(f'Last update: {checker.get_last_update()}\n')

    # Test IPs
    test_ips = [
        '185.220.101.1',  # Example Tor IP (may or may not be in list)
        '8.8.8.8',        # Google DNS (not Tor)
        '1.1.1.1',        # Cloudflare DNS (not Tor)
    ]

    print('Testing IP addresses:')
    for ip in test_ips:
        is_tor = checker.is_tor(ip)
        status = 'TOR EXIT NODE' if is_tor else 'Not Tor'
        print(f'  {ip}: {status}')

if __name__ == '__main__':
    main()
