#!/bin/bash

# Run all benchmarks and generate comparison table

echo "Running NoTorToday Benchmarks..."
echo "================================"
echo ""

cd "$(dirname "$0")"

# Run Node.js benchmark
echo "Running Node.js benchmark..."
node tests/node/benchmark.js > benchmark_node.json 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✓ Node.js benchmark completed"
else
    echo "✗ Node.js benchmark failed"
fi

# Run Python benchmark
echo "Running Python benchmark..."
python3 tests/python/benchmark.py > benchmark_python.json 2>/dev/null
if [ $? -eq 0 ]; then
    echo "✓ Python benchmark completed"
else
    echo "✗ Python benchmark failed"
fi

# Run Go benchmark
echo "Running Go benchmark..."
cd tests/go && go run benchmark.go > ../../benchmark_go.json 2>/dev/null
cd ../..
if [ $? -eq 0 ]; then
    echo "✓ Go benchmark completed"
else
    echo "✗ Go benchmark failed"
fi

# Run Java benchmark
echo "Running Java benchmark..."
cd tests/java && cp ../../core/java/NoTorToday.java . && javac NoTorToday.java Benchmark.java 2>/dev/null && java Benchmark > ../../benchmark_java.json 2>/dev/null
cd ../..
if [ $? -eq 0 ]; then
    echo "✓ Java benchmark completed"
else
    echo "✗ Java benchmark failed"
fi

# Run Kotlin benchmark (if kotlinc is available)
if command -v kotlinc &> /dev/null; then
    echo "Running Kotlin benchmark..."
    cd tests/kotlin && kotlinc -include-runtime -d benchmark.jar ../../core/kotlin/NoTorToday.kt benchmark.kt 2>/dev/null && java -jar benchmark.jar > ../../benchmark_kotlin.json 2>/dev/null
    cd ../..
    if [ $? -eq 0 ]; then
        echo "✓ Kotlin benchmark completed"
    else
        echo "✗ Kotlin benchmark failed"
    fi
else
    echo "⊘ Kotlin compiler not found, skipping"
fi

# Run Dart benchmark (if dart is available)
if command -v dart &> /dev/null; then
    echo "Running Dart benchmark..."
    cd tests/dart && dart benchmark.dart > ../../benchmark_dart.json 2>/dev/null
    cd ../..
    if [ $? -eq 0 ]; then
        echo "✓ Dart benchmark completed"
    else
        echo "✗ Dart benchmark failed"
    fi
else
    echo "⊘ Dart not found, skipping"
fi

echo ""
echo "Generating benchmark table..."

# Generate table using Python
python3 << 'EOF'
import json
import os
from datetime import datetime

# Read benchmark results
results = []
for lang in ['node', 'python', 'go', 'java', 'kotlin', 'dart']:
    try:
        with open(f'benchmark_{lang}.json', 'r') as f:
            data = json.load(f)
            results.append(data)
    except:
        print(f"Warning: Could not read benchmark for {lang}")

if not results:
    print("No benchmark results available")
    exit(0)

# Sort by ops_per_second descending
results.sort(key=lambda x: int(x.get('ops_per_second', 0)), reverse=True)

# Generate markdown table
table = "## Performance Benchmark\n\n"
table += f"*Last updated: {datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S')} UTC*\n\n"
table += "| Language | Duration (ms) | Operations | Ops/Second | Exit Nodes |\n"
table += "|----------|---------------|------------|------------|------------|\n"

for r in results:
    table += f"| {r['language']} | {r['duration_ms']} | {r['operations']:,} | {r['ops_per_second']:,} | {r['exit_nodes_loaded']:,} |\n"

# Read existing README or create new
readme_path = 'README.md'
if os.path.exists(readme_path):
    with open(readme_path, 'r') as f:
        content = f.read()
    
    # Replace or append benchmark section
    marker = "## Performance Benchmark"
    if marker in content:
        # Find the section and replace it
        start = content.find(marker)
        # Find the next ## section or end of file
        next_section = content.find('\n## ', start + 1)
        if next_section == -1:
            content = content[:start] + table
        else:
            content = content[:start] + table + '\n' + content[next_section:]
    else:
        # Append to end
        content += '\n\n' + table
else:
    content = table

with open(readme_path, 'w') as f:
    f.write(content)

print("✓ Benchmark table generated successfully")
print("")
print("Results:")
print(table)
EOF

# Clean up temporary files
rm -f benchmark_*.json

echo ""
echo "Done! Check README.md for the benchmark table."
