# NoTorToday 🔒

[![Tests](https://github.com/TheusHen/NoTorToday/actions/workflows/test.yml/badge.svg)](https://github.com/TheusHen/NoTorToday/actions/workflows/test.yml)
[![Benchmark](https://github.com/TheusHen/NoTorToday/actions/workflows/benchmark.yml/badge.svg)](https://github.com/TheusHen/NoTorToday/actions/workflows/benchmark.yml)
[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](http://unlicense.org/)

**NoTorToday** is a fast, lightweight, and universal library to check if an IP address belongs to a known Tor exit node. It works entirely offline after the initial download, with no network calls during lookups.

## 🎯 Features

- ✅ **Universal**: Identical API across multiple languages (Node.js, Python, Go, Rust)
- ⚡ **Fast**: O(1) lookup time using hash sets
- 🔄 **Auto-updating**: Automatically downloads and caches Tor exit node list
- 💾 **Offline-first**: Works without network after initial cache
- 🪶 **Lightweight**: ~200KB dataset, fully loaded in RAM
- 🌐 **IPv4 & IPv6**: Supports both IP address formats

## 📦 Installation

### Node.js

```bash
npm install notortoday
```

Or use the standalone file:
```bash
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/node/NoTorToday.js
```

### Python

```bash
pip install notortoday
```

Or use the standalone file:
```bash
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/python/notortoday.py
```

### Go

```bash
go get github.com/TheusHen/NoTorToday/core/go
```

### Rust

```toml
[dependencies]
notortoday = "0.1"
```

## 🚀 Quick Start

### Node.js

```javascript
const NoTorToday = require('./NoTorToday');

async function main() {
  // Initialize with auto-update
  const checker = new NoTorToday(true);
  await checker.updateList();
  
  // Check if IP is a Tor exit node
  console.log(checker.isTor('185.220.101.1')); // true or false
  console.log(checker.isTor('8.8.8.8'));        // false
  
  // Get statistics
  console.log(`Loaded ${checker.getCount()} exit nodes`);
  console.log(`Last update: ${checker.getLastUpdate()}`);
}

main();
```

### Python

```python
from notortoday import NoTorToday

# Initialize with auto-update
checker = NoTorToday(auto_update=True)

# Check if IP is a Tor exit node
print(checker.is_tor('185.220.101.1'))  # True or False
print(checker.is_tor('8.8.8.8'))         # False

# Get statistics
print(f'Loaded {checker.get_count()} exit nodes')
print(f'Last update: {checker.get_last_update()}')
```

### Go

```go
package main

import (
    "fmt"
    "github.com/TheusHen/NoTorToday/core/go"
)

func main() {
    // Initialize with auto-update
    checker, err := notortoday.New(true, "")
    if err != nil {
        panic(err)
    }
    
    // Check if IP is a Tor exit node
    fmt.Println(checker.IsTor("185.220.101.1")) // true or false
    fmt.Println(checker.IsTor("8.8.8.8"))        // false
    
    // Get statistics
    fmt.Printf("Loaded %d exit nodes\n", checker.GetCount())
}
```

### Rust

```rust
use notortoday::NoTorToday;

fn main() {
    // Initialize with auto-update
    let checker = NoTorToday::new(true, None).unwrap();
    
    // Check if IP is a Tor exit node
    println!("{}", checker.is_tor("185.220.101.1")); // true or false
    println!("{}", checker.is_tor("8.8.8.8"));        // false
    
    // Get statistics
    println!("Loaded {} exit nodes", checker.get_count());
}
```

## 📖 API Reference

All implementations provide the same interface:

### Constructor

- **Node.js**: `new NoTorToday(autoUpdate = true, cacheDir = null)`
- **Python**: `NoTorToday(auto_update=True, cache_dir=None)`
- **Go**: `New(autoUpdate bool, cacheDir string)`
- **Rust**: `NoTorToday::new(auto_update: bool, cache_dir: Option<String>)`

**Parameters:**
- `autoUpdate`: If true, automatically downloads the Tor exit node list on initialization
- `cacheDir`: Directory for cache file (defaults to current working directory)

### Methods

| Method | Description | Return Type |
|--------|-------------|-------------|
| `isTor(ip)` / `is_tor(ip)` / `IsTor(ip)` | Check if IP is a Tor exit node | Boolean |
| `updateList()` / `update_list()` / `UpdateList()` | Manually update the exit node list | Promise/void |
| `getCount()` / `get_count()` / `GetCount()` | Get number of loaded exit nodes | Number/int |
| `getLastUpdate()` / `get_last_update()` / `GetLastUpdate()` | Get last update timestamp | Date/datetime/Time |

## 🏗️ How It Works

1. **Download**: Fetches the official Tor exit node list from `https://check.torproject.org/exit-addresses`
2. **Parse**: Extracts IP addresses from lines starting with `ExitAddress`
3. **Cache**: Saves the list locally to `.notortoday_cache` for offline use
4. **Lookup**: Uses a hash set for O(1) constant-time IP lookups

## 🗂️ Project Structure

```
NoTorToday/
├── core/
│   ├── node/NoTorToday.js       # Node.js implementation
│   ├── python/notortoday.py     # Python implementation
│   ├── go/notortoday.go         # Go implementation
│   └── rust/src/lib.rs          # Rust implementation
├── examples/
│   ├── node/example.js
│   ├── python/example.py
│   ├── go/example.go
│   └── rust/example.rs
├── tests/
│   ├── node/benchmark.js
│   ├── python/benchmark.py
│   ├── go/benchmark.go
│   └── rust/benchmark.rs
└── .github/workflows/
    └── benchmark.yml            # Automated benchmarking
```

## ⚙️ Configuration

### Cache Management

The cache file `.notortoday_cache` is stored in:
- Current working directory (default)
- Custom directory if specified in constructor

### Manual Updates

```javascript
// Node.js
await checker.updateList();

// Python
checker.update_list()

// Go
checker.UpdateList()

// Rust
checker.update_list()
```

### Automatic Updates

For production use, consider:
- Running a cron job to update the cache daily
- Implementing TTL-based automatic updates
- Using the scheduled GitHub workflow to keep the list fresh

## 🔒 Security Considerations

- **Data Source**: Uses the official Tor Project exit node list
- **Update Frequency**: Tor exit nodes change frequently; update at least daily
- **Offline Fallback**: Works offline if the cache exists
- **No External Dependencies**: Minimal dependencies for security

## 🧪 Testing

Run the examples:

```bash
# Node.js
cd examples/node && node example-offline.js

# Python
cd examples/python && python3 example-offline.py

# Go
cd examples/go && go run example-offline.go
```

Run unit tests:

```bash
# Node.js
cd tests/node && node test.js

# Python
cd tests/python && python3 test.py

# Go
cd core/go && go test -v
```

Run benchmarks:

```bash
# Run all benchmarks and update README
./run-benchmarks.sh

# Or run individually
node tests/node/benchmark.js
python3 tests/python/benchmark.py
cd tests/go && go run benchmark.go
```

## 📊 Benchmarks

All implementations are benchmarked automatically on every push. The table below shows performance comparison across languages:

**Test Configuration:**
- 1,000 iterations × 5 IPs = 5,000 operations
- Measured: Duration (ms), Operations per second
- Dataset: 10 sample Tor exit nodes



## Performance Benchmark

*Last updated: 2025-11-05 00:15:50 UTC*

| Language | Duration (ms) | Operations | Ops/Second | Exit Nodes |
|----------|---------------|------------|------------|------------|
| Go | 0.05 | 5,000 | 103,391,232 | 10 |
| Python | 0.27 | 5,000 | 18,723,225 | 10 |
| Node.js | 0.63 | 5,000 | 7,890,007 | 10 |

## 🚀 Performance Tips

1. **Initialize once**: Create a single instance and reuse it throughout your application
2. **Pre-load data**: Initialize with `autoUpdate=true` at application startup
3. **Cache strategy**: Update the list periodically (e.g., daily) rather than on every request
4. **Memory footprint**: ~200KB for the full list of exit nodes (~3,000-5,000 IPs)

## 💡 Use Cases

- **Web Security**: Block or flag requests from Tor exit nodes
- **Rate Limiting**: Apply different rate limits for Tor traffic
- **Analytics**: Identify and filter Tor traffic in analytics
- **Fraud Detection**: Flag suspicious activity originating from Tor
- **Compliance**: Meet regulatory requirements for monitoring anonymous traffic
- **Research**: Analyze Tor network usage patterns

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Report bugs**: Open an issue describing the bug and how to reproduce it
2. **Suggest features**: Share your ideas for new features or improvements
3. **Submit PRs**: Fix bugs, add features, or improve documentation
4. **Add languages**: Implement NoTorToday in additional programming languages
5. **Improve performance**: Optimize existing implementations

### Adding a New Language

To add support for a new language:

1. Create `core/<language>/notortoday.<ext>`
2. Implement the core interface:
   - Constructor with `autoUpdate` and `cacheDir` parameters
   - `isTor(ip)` method
   - `updateList()` method
   - `getCount()` and `getLastUpdate()` methods
3. Add example in `examples/<language>/`
4. Add benchmark in `tests/<language>/benchmark.<ext>`
5. Update workflow to include the new language
6. Update README

## 📄 License

This project is released into the public domain under The Unlicense. See [LICENSE](LICENSE) for details.

## 🙏 Acknowledgments

- **Tor Project**: For maintaining the public exit node list
- Data source: https://check.torproject.org/exit-addresses

## ⚠️ Disclaimer

This library is provided for educational and security purposes. Users are responsible for complying with applicable laws and regulations regarding network traffic monitoring and privacy.

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/TheusHen/NoTorToday/issues)
- **Discussions**: [GitHub Discussions](https://github.com/TheusHen/NoTorToday/discussions)

---

**Made with ❤️ for the open-source community**
