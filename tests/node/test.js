const NoTorToday = require('../../core/node/NoTorToday');
const fs = require('fs');
const path = require('path');
const assert = require('assert');

// Simple test framework
const tests = [];
const test = (name, fn) => tests.push({ name, fn });

// Load sample data
const sampleData = fs.readFileSync(
  path.join(__dirname, '../../datasets/sample-exit-addresses.txt'),
  'utf8'
);

test('should parse exit addresses correctly', () => {
  const checker = new NoTorToday(false);
  checker.parseExitAddresses(sampleData);
  assert.strictEqual(checker.getCount(), 10, 'Should load 10 exit nodes');
});

test('should identify Tor IPs correctly', () => {
  const checker = new NoTorToday(false);
  checker.parseExitAddresses(sampleData);
  
  assert.strictEqual(checker.isTor('185.220.101.1'), true, '185.220.101.1 should be Tor');
  assert.strictEqual(checker.isTor('185.220.101.2'), true, '185.220.101.2 should be Tor');
  assert.strictEqual(checker.isTor('8.8.8.8'), false, '8.8.8.8 should not be Tor');
  assert.strictEqual(checker.isTor('1.1.1.1'), false, '1.1.1.1 should not be Tor');
});

test('should validate IPv4 addresses', () => {
  const checker = new NoTorToday(false);
  
  assert.strictEqual(checker.isValidIP('192.168.1.1'), true);
  assert.strictEqual(checker.isValidIP('255.255.255.255'), true);
  assert.strictEqual(checker.isValidIP('0.0.0.0'), true);
  assert.strictEqual(checker.isValidIP('256.1.1.1'), false);
  assert.strictEqual(checker.isValidIP('invalid'), false);
  assert.strictEqual(checker.isValidIP('192.168.1'), false);
});

test('should validate IPv6 addresses', () => {
  const checker = new NoTorToday(false);
  
  assert.strictEqual(checker.isValidIP('2001:0db8:85a3:0000:0000:8a2e:0370:7334'), true);
  assert.strictEqual(checker.isValidIP('::1'), true);
  assert.strictEqual(checker.isValidIP('invalid:ipv6'), false);
});

test('should handle cache operations', () => {
  const checker = new NoTorToday(false);
  checker.parseExitAddresses(sampleData);
  checker.saveCache(sampleData);
  
  const checker2 = new NoTorToday(false);
  const loaded = checker2.loadCache();
  
  assert.strictEqual(loaded, true, 'Should load cache successfully');
  assert.strictEqual(checker2.getCount(), 10, 'Should have same count after loading cache');
});

// Run tests
async function runTests() {
  console.log('Running NoTorToday Node.js Tests\n');
  
  let passed = 0;
  let failed = 0;
  
  for (const { name, fn } of tests) {
    try {
      await fn();
      console.log(`✓ ${name}`);
      passed++;
    } catch (error) {
      console.log(`✗ ${name}`);
      console.log(`  ${error.message}`);
      failed++;
    }
  }
  
  console.log(`\nResults: ${passed} passed, ${failed} failed`);
  process.exit(failed > 0 ? 1 : 0);
}

runTests();
