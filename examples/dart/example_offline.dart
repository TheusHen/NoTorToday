import 'dart:io';
import '../../../core/dart/notortoday.dart';

void main() async {
  print('NoTorToday - Dart Example (Offline Mode)\n');
  
  // Create instance without auto-update
  final checker = NoTorToday(autoUpdate: false);
  
  // Load sample data for testing
  try {
    final sampleData = await File('../../datasets/sample-exit-addresses.txt').readAsString();
    checker.parseExitAddresses(sampleData);
    
    print('Loaded ${checker.getCount()} Tor exit nodes');
    print('Using sample dataset for offline testing\n');
    
    // Test IPs
    final testIPs = [
      '185.220.101.1',  // In sample dataset - should be Tor
      '185.220.101.2',  // In sample dataset - should be Tor
      '8.8.8.8',        // Google DNS - not Tor
      '1.1.1.1',        // Cloudflare DNS - not Tor
    ];
    
    print('Testing IP addresses:');
    for (final ip in testIPs) {
      final isTor = checker.isTor(ip);
      final status = isTor ? 'TOR EXIT NODE ✓' : 'Not Tor ✗';
      print('  $ip: $status');
    }
  } catch (e) {
    stderr.writeln('Error: $e');
  }
}
