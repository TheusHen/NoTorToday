#ifndef NOTORTODAY_HPP
#define NOTORTODAY_HPP

#include <string>
#include <unordered_set>
#include <ctime>
#include <optional>

/**
 * NoTorToday - Check if an IP address belongs to a Tor exit node
 * 
 * Usage:
 *   NoTorToday checker(true);
 *   bool isTor = checker.isTor("185.220.101.1");
 */
class NoTorToday {
public:
    /**
     * Constructor
     * @param autoUpdate Whether to automatically download the Tor exit node list
     * @param cacheDir Directory for cache file (empty for current directory)
     */
    NoTorToday(bool autoUpdate = true, const std::string& cacheDir = "");
    
    /**
     * Destructor
     */
    ~NoTorToday() = default;
    
    /**
     * Download and parse the Tor exit node list
     * @return true on success, false on failure
     */
    bool updateList();
    
    /**
     * Check if an IP address is a Tor exit node
     * @param ip The IP address to check
     * @return true if the IP is a Tor exit node, false otherwise
     */
    bool isTor(const std::string& ip) const;
    
    /**
     * Get the number of loaded Tor exit nodes
     * @return The count of exit nodes
     */
    size_t getCount() const;
    
    /**
     * Get the last update timestamp
     * @return The last update time, or nullopt if never updated
     */
    std::optional<std::time_t> getLastUpdate() const;
    
    /**
     * Parse exit addresses from content (for testing with sample data)
     * @param content Content to parse
     */
    void parseExitAddresses(const std::string& content);

private:
    std::unordered_set<std::string> data;
    std::optional<std::time_t> lastUpdate;
    std::string cacheFile;
    static constexpr const char* TOR_LIST_URL = "https://check.torproject.org/exit-addresses";
    
    std::string downloadTorList();
    bool isValidIP(const std::string& ip) const;
    void saveCache(const std::string& content);
    bool loadCache();
};

#endif // NOTORTODAY_HPP
