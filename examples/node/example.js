const NoTorToday = require('../../core/node/NoTorToday');

async function main() {
  console.log('NoTorToday - Node.js Example\n');

  // Create instance with auto-update
  const checker = new NoTorToday(false);
  
  console.log('Updating Tor exit node list...');
  await checker.updateList();
  
  console.log(`Loaded ${checker.getCount()} Tor exit nodes`);
  console.log(`Last update: ${checker.getLastUpdate()}\n`);

  // Test IPs
  const testIPs = [
    '185.220.101.1',  // Example Tor IP (may or may not be in list)
    '8.8.8.8',        // Google DNS (not Tor)
    '1.1.1.1',        // Cloudflare DNS (not Tor)
  ];

  console.log('Testing IP addresses:');
  for (const ip of testIPs) {
    const isTor = checker.isTor(ip);
    console.log(`  ${ip}: ${isTor ? 'TOR EXIT NODE' : 'Not Tor'}`);
  }
}

main().catch(console.error);
