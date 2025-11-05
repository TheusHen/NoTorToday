# NoTorToday 🔒

[![Tests](https://github.com/TheusHen/NoTorToday/actions/workflows/test.yml/badge.svg)](https://github.com/TheusHen/NoTorToday/actions/workflows/test.yml)
[![Benchmark](https://github.com/TheusHen/NoTorToday/actions/workflows/benchmark.yml/badge.svg)](https://github.com/TheusHen/NoTorToday/actions/workflows/benchmark.yml)
[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](http://unlicense.org/)

**NoTorToday** is a fast, lightweight, and universal library to check if an IP address belongs to a known Tor exit node. It works entirely offline after the initial download, with no network calls during lookups.

**[📖 Quick Start Guide](QUICKSTART.md)** | **[🤝 Contributing](CONTRIBUTING.md)** | **[📊 Benchmarks](#performance-benchmark)**

## 🎯 Features

- ✅ **Universal**: Identical API across 10+ languages (Node.js, Python, Go, Rust, Java, Kotlin, C, C++, C#, Dart)
- ⚡ **Fast**: O(1) lookup time using hash sets
- 🔄 **Auto-updating**: Automatically downloads and caches Tor exit node list
- 💾 **Offline-first**: Works without network after initial cache
- 🪶 **Lightweight**: ~200KB dataset, fully loaded in RAM
- 🌐 **IPv4 & IPv6**: Supports both IP address formats
- 📱 **Mobile & Desktop**: Full support for mobile (Kotlin, Dart/Flutter), web, and desktop platforms

## 📦 Installation

**NoTorToday uses a manual download approach** - simply copy the implementation file for your language into your project.

### Node.js

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/node/NoTorToday.js

# Or manually download and place in your project's lib/ folder
# Then import:
const NoTorToday = require('./lib/NoTorToday');
```

### Python

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/python/notortoday.py

# Or manually download and place in your project
# Then import:
from lib.notortoday import NoTorToday
```

### Go

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/go/notortoday.go

# Place in your project and import as a local package
```

### Java

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/java/NoTorToday.java

# Compile and use:
javac NoTorToday.java
```

### Kotlin

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/kotlin/NoTorToday.kt

# Add to your project's src folder
```

### C

```bash
# Download the files
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/c/notortoday.h
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/c/notortoday.c

# Compile with:
gcc -o myapp myapp.c notortoday.c -lcurl
```

### C++

```bash
# Download the files
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/cpp/notortoday.hpp
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/cpp/notortoday.cpp

# Compile with:
g++ -std=c++17 -o myapp myapp.cpp notortoday.cpp -lcurl
```

### C#

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/csharp/NoTorToday.cs

# Add to your project and compile:
csc /out:myapp.exe myapp.cs NoTorToday.cs
```

### Dart (Flutter)

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/dart/notortoday.dart

# Add to your project's lib/ folder
# Then import:
import 'package:myapp/lib/notortoday.dart';
```

### Rust

```bash
# Download the file
curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/rust/src/lib.rs

# Add to your project's src folder
```

## 🚀 Quick Start

All implementations provide the same interface with language-appropriate naming conventions:

### Node.js

```javascript
const NoTorToday = require('./lib/NoTorToday');

async function main() {
  const checker = new NoTorToday(true);
  await checker.updateList();
  
  console.log(checker.isTor('185.220.101.1')); // true or false
  console.log(checker.isTor('8.8.8.8'));        // false
  
  console.log(`Loaded ${checker.getCount()} exit nodes`);
}

main();
```

### Python

```python
from lib.notortoday import NoTorToday

checker = NoTorToday(auto_update=True)

print(checker.is_tor('185.220.101.1'))  # True or False
print(checker.is_tor('8.8.8.8'))         # False

print(f'Loaded {checker.get_count()} exit nodes')
```

### Java

```java
NoTorToday checker = new NoTorToday(true);

System.out.println(checker.isTor("185.220.101.1")); // true or false
System.out.println(checker.isTor("8.8.8.8"));        // false

System.out.println("Loaded " + checker.getCount() + " exit nodes");
```

### Kotlin

```kotlin
val checker = NoTorToday(autoUpdate = true)

println(checker.isTor("185.220.101.1")) // true or false
println(checker.isTor("8.8.8.8"))        // false

println("Loaded ${checker.getCount()} exit nodes")
```

### C

```c
#include "notortoday.h"

NoTorToday* checker = notortoday_new(true, NULL);

printf("%d\n", notortoday_is_tor(checker, "185.220.101.1")); // 1 or 0
printf("%d\n", notortoday_is_tor(checker, "8.8.8.8"));        // 0

printf("Loaded %d exit nodes\n", notortoday_get_count(checker));

notortoday_free(checker);
```

### C++

```cpp
#include "notortoday.hpp"

NoTorToday checker(true);

std::cout << checker.isTor("185.220.101.1") << "\n"; // true or false
std::cout << checker.isTor("8.8.8.8") << "\n";        // false

std::cout << "Loaded " << checker.getCount() << " exit nodes\n";
```

### C#

```csharp
var checker = new NoTorToday(autoUpdate: true);
await checker.UpdateListAsync();

Console.WriteLine(checker.IsTor("185.220.101.1")); // true or false
Console.WriteLine(checker.IsTor("8.8.8.8"));        // false

Console.WriteLine($"Loaded {checker.GetCount()} exit nodes");
```

### Dart (Flutter)

```dart
final checker = NoTorToday(autoUpdate: true);
await checker.updateList();

print(checker.isTor('185.220.101.1')); // true or false
print(checker.isTor('8.8.8.8'));        // false

print('Loaded ${checker.getCount()} exit nodes');
```

### Go

```go
import "yourproject/notortoday"

checker, _ := notortoday.New(true, "")

fmt.Println(checker.IsTor("185.220.101.1")) // true or false
fmt.Println(checker.IsTor("8.8.8.8"))        // false

fmt.Printf("Loaded %d exit nodes\n", checker.GetCount())
```

### Rust

```rust
use notortoday::NoTorToday;

let checker = NoTorToday::new(true, None).unwrap();

println!("{}", checker.is_tor("185.220.101.1")); // true or false
println!("{}", checker.is_tor("8.8.8.8"));        // false

println!("Loaded {} exit nodes", checker.get_count());
```

## 📖 API Reference

All implementations provide the same interface:

### Constructor

- **Node.js**: `new NoTorToday(autoUpdate = true, cacheDir = null)`
- **Python**: `NoTorToday(auto_update=True, cache_dir=None)`
- **Go**: `New(autoUpdate bool, cacheDir string)`
- **Rust**: `NoTorToday::new(auto_update: bool, cache_dir: Option<String>)`
- **Java**: `new NoTorToday(boolean autoUpdate, String cacheDir)`
- **Kotlin**: `NoTorToday(autoUpdate: Boolean = true, cacheDir: String? = null)`
- **C**: `notortoday_new(bool auto_update, const char* cache_dir)`
- **C++**: `NoTorToday(bool autoUpdate = true, const std::string& cacheDir = "")`
- **C#**: `new NoTorToday(bool autoUpdate = true, string cacheDir = null)`
- **Dart**: `NoTorToday({bool autoUpdate = true, String? cacheDir})`

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
│   ├── rust/src/lib.rs          # Rust implementation
│   ├── java/NoTorToday.java     # Java implementation
│   ├── kotlin/NoTorToday.kt     # Kotlin implementation
│   ├── c/notortoday.{h,c}       # C implementation
│   ├── cpp/notortoday.{hpp,cpp} # C++ implementation
│   ├── csharp/NoTorToday.cs     # C# implementation
│   └── dart/notortoday.dart     # Dart/Flutter implementation
├── examples/
│   └── <language>/example_offline.*
├── tests/
│   └── <language>/
└── datasets/
    └── sample-exit-addresses.txt
```

## ⚙️ Configuration

### Cache Management

The cache file `.notortoday_cache` is stored in:
- Current working directory (default)
- Custom directory if specified in constructor

### Manual Updates

Update the Tor exit node list manually:

```javascript
// Node.js
await checker.updateList();

// Python
checker.update_list()

// Go
checker.UpdateList()

// Java/Kotlin/C#
checker.updateList() // or UpdateList()

// C
notortoday_update_list(checker)

// C++
checker.updateList()

// Dart
await checker.updateList()

// Rust
checker.update_list()
```

## 🔒 Security Considerations

- **Data Source**: Uses the official Tor Project exit node list
- **Update Frequency**: Tor exit nodes change frequently; update at least daily
- **Offline Fallback**: Works offline if the cache exists
- **Dependencies**: Minimal external dependencies (varies by language)

## 🧪 Testing

Run the examples:

```bash
# Node.js
cd examples/node && node example-offline.js

# Python
cd examples/python && python3 example-offline.py

# Go
cd examples/go && go run example-offline.go

# Java
cd examples/java && cp ../../core/java/NoTorToday.java . && javac NoTorToday.java ExampleOffline.java && java ExampleOffline

# Kotlin
cd examples/kotlin && kotlinc -include-runtime -d example.jar ../../core/kotlin/NoTorToday.kt example_offline.kt && java -jar example.jar

# C
cd examples/c && gcc -o example example_offline.c ../../core/c/notortoday.c -lcurl && ./example

# C++
cd examples/cpp && g++ -std=c++17 -o example example_offline.cpp ../../core/cpp/notortoday.cpp -lcurl && ./example

# C#
cd examples/csharp && csc /out:example.exe ../../core/csharp/NoTorToday.cs example_offline.cs && mono example.exe

# Dart
cd examples/dart && dart example_offline.dart
```

## 📊 Benchmarks

All implementations are benchmarked automatically. See the performance comparison below:

## Performance Benchmark

*Last updated: 2025-11-05 00:58:41 UTC*

| Language | Duration (ms) | Operations | Ops/Second | Exit Nodes |
|----------|---------------|------------|------------|------------|
| Go | 0.05 | 5,000 | 100,617,793 | 10 |
| Python | 0.27 | 5,000 | 18,764,120 | 10 |
| Node.js | 0.58 | 5,000 | 8,667,855 | 10 |
| Java | 1.43 | 5,000 | 3,499,268 | 10 |
| Kotlin | 1.67 | 5,000 | 3,000,255 | 10 |


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
- **Mobile Apps**: Kotlin for Android, Dart/Flutter for cross-platform
- **Desktop Apps**: C/C++/C# for native desktop applications

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
