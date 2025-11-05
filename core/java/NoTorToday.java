import java.io.*;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.nio.file.*;
import java.time.Instant;
import java.util.*;

/**
 * NoTorToday - Check if an IP address belongs to a Tor exit node
 * 
 * Usage:
 *   NoTorToday checker = new NoTorToday(true);
 *   boolean isTor = checker.isTor("185.220.101.1");
 */
public class NoTorToday {
    private Set<String> data;
    private Instant lastUpdate;
    private String cacheFile;
    private static final String TOR_LIST_URL = "https://check.torproject.org/exit-addresses";
    
    /**
     * Constructor
     * @param autoUpdate Whether to automatically download the Tor exit node list
     */
    public NoTorToday(boolean autoUpdate) {
        this(autoUpdate, null);
    }
    
    /**
     * Constructor with custom cache directory
     * @param autoUpdate Whether to automatically download the Tor exit node list
     * @param cacheDir Directory for cache file (null for current directory)
     */
    public NoTorToday(boolean autoUpdate, String cacheDir) {
        this.data = new HashSet<>();
        this.lastUpdate = null;
        
        if (cacheDir == null) {
            cacheDir = System.getProperty("user.dir");
        }
        this.cacheFile = Paths.get(cacheDir, ".notortoday_cache").toString();
        
        if (autoUpdate) {
            try {
                updateList();
            } catch (IOException e) {
                // Try to load from cache if update fails
                if (!loadCache()) {
                    System.err.println("Failed to update and no cache available: " + e.getMessage());
                }
            }
        } else {
            loadCache();
        }
    }
    
    /**
     * Download and parse the Tor exit node list
     * @throws IOException if download fails
     */
    public void updateList() throws IOException {
        String content = downloadTorList();
        parseExitAddresses(content);
        saveCache(content);
        this.lastUpdate = Instant.now();
    }
    
    /**
     * Download Tor exit node list from torproject.org
     * @return The downloaded content
     * @throws IOException if download fails
     */
    private String downloadTorList() throws IOException {
        URL url = new URL(TOR_LIST_URL);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");
        conn.setConnectTimeout(10000);
        conn.setReadTimeout(10000);
        
        try (BufferedReader reader = new BufferedReader(
                new InputStreamReader(conn.getInputStream()))) {
            StringBuilder content = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                content.append(line).append("\n");
            }
            return content.toString();
        } finally {
            conn.disconnect();
        }
    }
    
    /**
     * Parse exit addresses from the downloaded data
     * @param content The content to parse
     */
    public void parseExitAddresses(String content) {
        data.clear();
        String[] lines = content.split("\n");
        
        for (String line : lines) {
            if (line.startsWith("ExitAddress ")) {
                String[] parts = line.split("\\s+");
                if (parts.length >= 2) {
                    String ip = parts[1].trim();
                    if (isValidIP(ip)) {
                        data.add(ip);
                    }
                }
            }
        }
    }
    
    /**
     * Validate IP address (IPv4 and IPv6)
     * @param ip The IP address to validate
     * @return true if valid, false otherwise
     */
    private boolean isValidIP(String ip) {
        try {
            InetAddress.getByName(ip);
            return true;
        } catch (UnknownHostException e) {
            return false;
        }
    }
    
    /**
     * Check if an IP address is a Tor exit node
     * @param ip The IP address to check
     * @return true if the IP is a Tor exit node, false otherwise
     */
    public boolean isTor(String ip) {
        return data.contains(ip);
    }
    
    /**
     * Get the number of loaded Tor exit nodes
     * @return The count of exit nodes
     */
    public int getCount() {
        return data.size();
    }
    
    /**
     * Get the last update timestamp
     * @return The last update time, or null if never updated
     */
    public Instant getLastUpdate() {
        return lastUpdate;
    }
    
    /**
     * Save cache to disk
     * @param content The content to save
     */
    private void saveCache(String content) {
        try {
            Files.write(Paths.get(cacheFile), content.getBytes());
        } catch (IOException e) {
            // Silently fail if cache can't be written
        }
    }
    
    /**
     * Load cache from disk
     * @return true if cache was loaded successfully, false otherwise
     */
    private boolean loadCache() {
        try {
            Path path = Paths.get(cacheFile);
            if (Files.exists(path)) {
                String content = new String(Files.readAllBytes(path));
                parseExitAddresses(content);
                this.lastUpdate = Files.getLastModifiedTime(path).toInstant();
                return true;
            }
        } catch (IOException e) {
            // Silently fail if cache can't be read
        }
        return false;
    }
}
