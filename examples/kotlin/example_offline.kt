import java.io.File

fun main() {
    println("NoTorToday - Kotlin Example (Offline Mode)\n")
    
    // Create instance without auto-update
    val checker = NoTorToday(autoUpdate = false)
    
    // Load sample data for testing
    try {
        val sampleData = File("../../datasets/sample-exit-addresses.txt").readText()
        checker.parseExitAddresses(sampleData)
        
        println("Loaded ${checker.getCount()} Tor exit nodes")
        println("Using sample dataset for offline testing\n")
        
        // Test IPs
        val testIPs = listOf(
            "185.220.101.1",  // In sample dataset - should be Tor
            "185.220.101.2",  // In sample dataset - should be Tor
            "8.8.8.8",        // Google DNS - not Tor
            "1.1.1.1"         // Cloudflare DNS - not Tor
        )
        
        println("Testing IP addresses:")
        for (ip in testIPs) {
            val isTor = checker.isTor(ip)
            val status = if (isTor) "TOR EXIT NODE ✓" else "Not Tor ✗"
            println("  $ip: $status")
        }
    } catch (e: Exception) {
        System.err.println("Error: ${e.message}")
    }
}
