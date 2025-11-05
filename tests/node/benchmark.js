const NoTorToday = require('../../core/node/NoTorToday');
const fs = require('fs');
const path = require('path');

async function benchmark() {
  const iterations = 1000;
  
  // Initialize
  const checker = new NoTorToday(false);
  
  // Load sample data for offline testing
  try {
    const sampleData = fs.readFileSync(
      path.join(__dirname, '../../datasets/sample-exit-addresses.txt'),
      'utf8'
    );
    checker.parseExitAddresses(sampleData);
  } catch (e) {
    // Try to download if sample not available
    try {
      await checker.updateList();
    } catch (err) {
      console.error('Cannot load data:', err.message);
      process.exit(1);
    }
  }
  
  const testIPs = [
    '185.220.101.1',
    '8.8.8.8',
    '1.1.1.1',
    '192.168.1.1',
    '10.0.0.1',
  ];
  
  // Warmup
  for (let i = 0; i < 100; i++) {
    testIPs.forEach(ip => checker.isTor(ip));
  }
  
  // Benchmark
  const start = process.hrtime.bigint();
  
  for (let i = 0; i < iterations; i++) {
    testIPs.forEach(ip => checker.isTor(ip));
  }
  
  const end = process.hrtime.bigint();
  const durationMs = Number(end - start) / 1_000_000;
  const opsPerSec = (iterations * testIPs.length) / (durationMs / 1000);
  
  console.log(JSON.stringify({
    language: 'Node.js',
    duration_ms: durationMs.toFixed(2),
    operations: iterations * testIPs.length,
    ops_per_second: Math.round(opsPerSec),
    exit_nodes_loaded: checker.getCount()
  }));
}

benchmark().catch(console.error);
