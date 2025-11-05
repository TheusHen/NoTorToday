import 'dart:io';
import '../../core/dart/notortoday.dart';

void main() async {
  const iterations = 1000;
  
  // Initialize
  final checker = NoTorToday(autoUpdate: false);
  
  // Load sample data for offline testing
  try {
    final sampleData = await File('../../datasets/sample-exit-addresses.txt').readAsString();
    checker.parseExitAddresses(sampleData);
  } catch (e) {
    stderr.writeln('Cannot load data: $e');
    exit(1);
  }
  
  final testIPs = [
    '185.220.101.1',
    '8.8.8.8',
    '1.1.1.1',
    '192.168.1.1',
    '10.0.0.1',
  ];
  
  // Warmup
  for (var i = 0; i < 100; i++) {
    for (final ip in testIPs) {
      checker.isTor(ip);
    }
  }
  
  // Benchmark
  final stopwatch = Stopwatch()..start();
  
  for (var i = 0; i < iterations; i++) {
    for (final ip in testIPs) {
      checker.isTor(ip);
    }
  }
  
  stopwatch.stop();
  final durationMs = stopwatch.elapsedMicroseconds / 1000.0;
  final opsPerSec = ((iterations * testIPs.length) / (durationMs / 1000.0)).round();
  
  // Output JSON
  print('{"language":"Dart","duration_ms":"${durationMs.toStringAsFixed(2)}","operations":${iterations * testIPs.length},"ops_per_second":$opsPerSec,"exit_nodes_loaded":${checker.getCount()}}');
}
