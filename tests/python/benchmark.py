import sys
import os
import time
import json

# Add core directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../core/python'))

from notortoday import NoTorToday

def benchmark():
    iterations = 1000
    
    # Initialize
    checker = NoTorToday(auto_update=False)
    
    # Load sample data for offline testing
    try:
        sample_path = os.path.join(os.path.dirname(__file__), 
                                   '../../datasets/sample-exit-addresses.txt')
        with open(sample_path, 'r') as f:
            sample_data = f.read()
        checker.parse_exit_addresses(sample_data)
    except:
        # Try to download if sample not available
        try:
            checker.update_list()
        except Exception as e:
            print(f'Cannot load data: {e}', file=sys.stderr)
            sys.exit(1)
    
    test_ips = [
        '185.220.101.1',
        '8.8.8.8',
        '1.1.1.1',
        '192.168.1.1',
        '10.0.0.1',
    ]
    
    # Warmup
    for _ in range(100):
        for ip in test_ips:
            checker.is_tor(ip)
    
    # Benchmark
    start = time.perf_counter()
    
    for _ in range(iterations):
        for ip in test_ips:
            checker.is_tor(ip)
    
    end = time.perf_counter()
    duration_ms = (end - start) * 1000
    ops_per_sec = (iterations * len(test_ips)) / (duration_ms / 1000)
    
    result = {
        'language': 'Python',
        'duration_ms': f'{duration_ms:.2f}',
        'operations': iterations * len(test_ips),
        'ops_per_second': int(ops_per_sec),
        'exit_nodes_loaded': checker.get_count()
    }
    
    print(json.dumps(result))

if __name__ == '__main__':
    benchmark()
