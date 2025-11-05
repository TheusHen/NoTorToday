import java.io.File
import java.net.HttpURLConnection
import java.net.InetAddress
import java.net.URL
import java.time.Instant

/**
 * NoTorToday - Check if an IP address belongs to a Tor exit node
 * 
 * Usage:
 *   val checker = NoTorToday(autoUpdate = true)
 *   val isTor = checker.isTor("185.220.101.1")
 */
class NoTorToday(autoUpdate: Boolean = true, cacheDir: String? = null) {
    private val data: MutableSet<String> = mutableSetOf()
    private var lastUpdate: Instant? = null
    private val cacheFile: String
    
    companion object {
        private const val TOR_LIST_URL = "https://check.torproject.org/exit-addresses"
    }
    
    init {
        val dir = cacheDir ?: System.getProperty("user.dir")
        cacheFile = File(dir, ".notortoday_cache").absolutePath
        
        if (autoUpdate) {
            try {
                updateList()
            } catch (e: Exception) {
                // Try to load from cache if update fails
                if (!loadCache()) {
                    System.err.println("Failed to update and no cache available: ${e.message}")
                }
            }
        } else {
            loadCache()
        }
    }
    
    /**
     * Download and parse the Tor exit node list
     */
    fun updateList() {
        val content = downloadTorList()
        parseExitAddresses(content)
        saveCache(content)
        lastUpdate = Instant.now()
    }
    
    /**
     * Download Tor exit node list from torproject.org
     */
    private fun downloadTorList(): String {
        val url = URL(TOR_LIST_URL)
        val connection = url.openConnection() as HttpURLConnection
        connection.requestMethod = "GET"
        connection.connectTimeout = 10000
        connection.readTimeout = 10000
        
        return try {
            connection.inputStream.bufferedReader().use { it.readText() }
        } finally {
            connection.disconnect()
        }
    }
    
    /**
     * Parse exit addresses from the downloaded data
     */
    fun parseExitAddresses(content: String) {
        data.clear()
        content.lines().forEach { line ->
            if (line.startsWith("ExitAddress ")) {
                val parts = line.split("\\s+".toRegex())
                if (parts.size >= 2) {
                    val ip = parts[1].trim()
                    if (isValidIP(ip)) {
                        data.add(ip)
                    }
                }
            }
        }
    }
    
    /**
     * Validate IP address (IPv4 and IPv6)
     */
    private fun isValidIP(ip: String): Boolean {
        return try {
            InetAddress.getByName(ip)
            true
        } catch (e: Exception) {
            false
        }
    }
    
    /**
     * Check if an IP address is a Tor exit node
     */
    fun isTor(ip: String): Boolean {
        return data.contains(ip)
    }
    
    /**
     * Get the number of loaded Tor exit nodes
     */
    fun getCount(): Int {
        return data.size
    }
    
    /**
     * Get the last update timestamp
     */
    fun getLastUpdate(): Instant? {
        return lastUpdate
    }
    
    /**
     * Save cache to disk
     */
    private fun saveCache(content: String) {
        try {
            File(cacheFile).writeText(content)
        } catch (e: Exception) {
            // Silently fail if cache can't be written
        }
    }
    
    /**
     * Load cache from disk
     */
    private fun loadCache(): Boolean {
        return try {
            val file = File(cacheFile)
            if (file.exists()) {
                val content = file.readText()
                parseExitAddresses(content)
                lastUpdate = Instant.ofEpochMilli(file.lastModified())
                true
            } else {
                false
            }
        } catch (e: Exception) {
            false
        }
    }
}
