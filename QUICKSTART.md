# Quick Start Guide

This guide will help you get started with NoTorToday in under 5 minutes.

## Installation

Choose your language and follow the instructions:

### Node.js

1. **Download the core file**:
   ```bash
   curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/node/NoTorToday.js
   ```

2. **Use in your code**:
   ```javascript
   const NoTorToday = require('./NoTorToday');
   
   async function checkIP(ip) {
     const checker = new NoTorToday(true);
     await checker.updateList();
     
     if (checker.isTor(ip)) {
       console.log('⚠️  Tor exit node detected!');
     } else {
       console.log('✓ Regular IP');
     }
   }
   
   checkIP('185.220.101.1');
   ```

### Python

1. **Download the core file**:
   ```bash
   curl -O https://raw.githubusercontent.com/TheusHen/NoTorToday/main/core/python/notortoday.py
   ```

2. **Use in your code**:
   ```python
   from notortoday import NoTorToday
   
   def check_ip(ip):
       checker = NoTorToday(auto_update=True)
       
       if checker.is_tor(ip):
           print('⚠️  Tor exit node detected!')
       else:
           print('✓ Regular IP')
   
   check_ip('185.220.101.1')
   ```

### Go

1. **Install the package**:
   ```bash
   go get github.com/TheusHen/NoTorToday/core/go
   ```

2. **Use in your code**:
   ```go
   package main
   
   import (
       "fmt"
       "github.com/TheusHen/NoTorToday/core/go"
   )
   
   func checkIP(ip string) {
       checker, _ := notortoday.New(true, "")
       
       if checker.IsTor(ip) {
           fmt.Println("⚠️  Tor exit node detected!")
       } else {
           fmt.Println("✓ Regular IP")
       }
   }
   
   func main() {
       checkIP("185.220.101.1")
   }
   ```

## Basic Usage Patterns

### Pattern 1: One-time Check

```javascript
// Node.js
const checker = new NoTorToday(true);
await checker.updateList();
console.log(checker.isTor('8.8.8.8')); // false
```

```python
# Python
checker = NoTorToday(auto_update=True)
print(checker.is_tor('8.8.8.8'))  # False
```

```go
// Go
checker, _ := notortoday.New(true, "")
fmt.Println(checker.IsTor("8.8.8.8"))  // false
```

### Pattern 2: Reusable Instance (Recommended)

```javascript
// Node.js - Initialize once at app startup
const torChecker = new NoTorToday(true);
await torChecker.updateList();

// Use throughout your application
app.use((req, res, next) => {
  const ip = req.ip;
  if (torChecker.isTor(ip)) {
    // Handle Tor traffic
    console.log('Tor user detected');
  }
  next();
});
```

```python
# Python - Initialize once at app startup
tor_checker = NoTorToday(auto_update=True)

# Use throughout your application
@app.before_request
def check_tor():
    ip = request.remote_addr
    if tor_checker.is_tor(ip):
        # Handle Tor traffic
        print('Tor user detected')
```

```go
// Go - Initialize once at app startup
var torChecker *notortoday.NoTorToday

func init() {
    torChecker, _ = notortoday.New(true, "")
}

// Use throughout your application
func handler(w http.ResponseWriter, r *http.Request) {
    ip := r.RemoteAddr
    if torChecker.IsTor(ip) {
        // Handle Tor traffic
        fmt.Println("Tor user detected")
    }
}
```

### Pattern 3: Manual Updates

```javascript
// Node.js - Update on schedule
setInterval(async () => {
  await checker.updateList();
  console.log(`Updated: ${checker.getCount()} exit nodes`);
}, 24 * 60 * 60 * 1000); // Every 24 hours
```

```python
# Python - Update on schedule
import schedule
import time

def update_list():
    checker.update_list()
    print(f'Updated: {checker.get_count()} exit nodes')

schedule.every(24).hours.do(update_list)

while True:
    schedule.run_pending()
    time.sleep(3600)
```

## Web Framework Integration Examples

### Express (Node.js)

```javascript
const express = require('express');
const NoTorToday = require('./NoTorToday');

const app = express();
const torChecker = new NoTorToday(true);

app.use(async (req, res, next) => {
  if (!torChecker.getCount()) {
    await torChecker.updateList();
  }
  
  const ip = req.ip || req.connection.remoteAddress;
  if (torChecker.isTor(ip)) {
    req.isTor = true;
    // Optional: Block or rate-limit
    // return res.status(403).send('Tor access not allowed');
  }
  next();
});

app.listen(3000);
```

### Flask (Python)

```python
from flask import Flask, request
from notortoday import NoTorToday

app = Flask(__name__)
tor_checker = NoTorToday(auto_update=True)

@app.before_request
def check_tor_request():
    ip = request.remote_addr
    if tor_checker.is_tor(ip):
        request.is_tor = True
        # Optional: Block or rate-limit
        # return 'Tor access not allowed', 403

@app.route('/')
def index():
    is_tor = getattr(request, 'is_tor', False)
    return f'Tor: {is_tor}'

app.run()
```

### Gin (Go)

```go
package main

import (
    "github.com/gin-gonic/gin"
    "github.com/TheusHen/NoTorToday/core/go"
)

var torChecker *notortoday.NoTorToday

func main() {
    torChecker, _ = notortoday.New(true, "")
    
    r := gin.Default()
    r.Use(checkTorMiddleware())
    
    r.GET("/", func(c *gin.Context) {
        isTor := c.GetBool("isTor")
        c.JSON(200, gin.H{"tor": isTor})
    })
    
    r.Run()
}

func checkTorMiddleware() gin.HandlerFunc {
    return func(c *gin.Context) {
        ip := c.ClientIP()
        if torChecker.IsTor(ip) {
            c.Set("isTor", true)
            // Optional: Block or rate-limit
            // c.AbortWithStatus(403)
            // return
        }
        c.Next()
    }
}
```

## Testing Your Integration

Run the provided examples to verify everything works:

```bash
# Clone the repository
git clone https://github.com/TheusHen/NoTorToday.git
cd NoTorToday

# Run examples
node examples/node/example-offline.js
python3 examples/python/example-offline.py
cd examples/go && go run example-offline.go
```

## Next Steps

- Read the [full documentation](README.md)
- Check out [Contributing Guidelines](CONTRIBUTING.md)
- Run benchmarks: `./run-benchmarks.sh`
- Review the [GitHub Workflows](.github/workflows/)

## Common Issues

### Issue: "Cannot download Tor list"
**Solution**: The library works offline! Use the cached version or manually update during off-peak hours.

### Issue: "High memory usage"
**Solution**: The full list is ~200KB and contains ~3,000-5,000 IPs. This is expected and optimal for performance.

### Issue: "IPv6 not detected"
**Solution**: Ensure you're using the full IPv6 address format, not the compressed version.

## Support

- [GitHub Issues](https://github.com/TheusHen/NoTorToday/issues)
- [GitHub Discussions](https://github.com/TheusHen/NoTorToday/discussions)

Happy coding! 🚀
