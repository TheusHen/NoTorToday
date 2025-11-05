const NoTorToday = require('../../core/node/NoTorToday');
const fs = require('fs');
const path = require('path');

async function main() {
  console.log('NoTorToday - Node.js Example (Offline Mode)\n');

  // Create instance without auto-update
  const checker = new NoTorToday(false);
  
  // Load sample data for testing
  const sampleData = fs.readFileSync(
    path.join(__dirname, '../../datasets/sample-exit-addresses.txt'),
    'utf8'
  );
  checker.parseExitAddresses(sampleData);
  
  console.log(`Loaded ${checker.getCount()} Tor exit nodes`);
  console.log(`Using sample dataset for offline testing\n`);

  // Test IPs
  const testIPs = [
    '185.220.101.1',  // In sample dataset - should be Tor
    '185.220.101.2',  // In sample dataset - should be Tor
    '8.8.8.8',        // Google DNS - not Tor
    '1.1.1.1',        // Cloudflare DNS - not Tor
  ];

  console.log('Testing IP addresses:');
  for (const ip of testIPs) {
    const isTor = checker.isTor(ip);
    console.log(`  ${ip}: ${isTor ? 'TOR EXIT NODE ✓' : 'Not Tor ✗'}`);
  }
}

main().catch(console.error);
