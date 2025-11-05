# NoTorToday 🔒

[![Benchmark](https://github.com/TheusHen/NoTorToday/actions/workflows/benchmark.yml/badge.svg)](https://github.com/TheusHen/NoTorToday/actions/workflows/benchmark.yml)

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
cd examples/node && node example.js

# Python
cd examples/python && python3 example.py

# Go
cd examples/go && go run example.go
```

## 📊 Benchmarks

All implementations are benchmarked automatically on every push. See the benchmark workflow results below:



## Performance Benchmark

*Last updated: 2025-11-05 00:15:50 UTC*

| Language | Duration (ms) | Operations | Ops/Second | Exit Nodes |
|----------|---------------|------------|------------|------------|
| Go | 0.05 | 5,000 | 103,391,232 | 10 |
| Python | 0.27 | 5,000 | 18,723,225 | 10 |
| Node.js | 0.63 | 5,000 | 7,890,007 | 10 |
