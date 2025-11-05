#include "notortoday.hpp"
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// Helper for HTTP download
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

NoTorToday::NoTorToday(bool autoUpdate, const std::string& cacheDir) {
    std::string dir = cacheDir.empty() ? fs::current_path().string() : cacheDir;
    cacheFile = (fs::path(dir) / ".notortoday_cache").string();
    
    if (autoUpdate) {
        if (!updateList()) {
            loadCache();
        }
    } else {
        loadCache();
    }
}

std::string NoTorToday::downloadTorList() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, TOR_LIST_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        throw std::runtime_error(std::string("CURL error: ") + curl_easy_strerror(res));
    }
    
    return readBuffer;
}

void NoTorToday::parseExitAddresses(const std::string& content) {
    data.clear();
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line.find("ExitAddress ") == 0) {
            std::istringstream lineStream(line);
            std::string prefix, ip;
            lineStream >> prefix >> ip;
            
            if (!ip.empty() && isValidIP(ip)) {
                data.insert(ip);
            }
        }
    }
}

bool NoTorToday::isValidIP(const std::string& ip) const {
    struct sockaddr_in sa4;
    struct sockaddr_in6 sa6;
    
    return inet_pton(AF_INET, ip.c_str(), &(sa4.sin_addr)) == 1 ||
           inet_pton(AF_INET6, ip.c_str(), &(sa6.sin6_addr)) == 1;
}

bool NoTorToday::updateList() {
    try {
        std::string content = downloadTorList();
        parseExitAddresses(content);
        saveCache(content);
        lastUpdate = std::time(nullptr);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Update failed: " << e.what() << std::endl;
        return false;
    }
}

bool NoTorToday::isTor(const std::string& ip) const {
    return data.find(ip) != data.end();
}

size_t NoTorToday::getCount() const {
    return data.size();
}

std::optional<std::time_t> NoTorToday::getLastUpdate() const {
    return lastUpdate;
}

void NoTorToday::saveCache(const std::string& content) {
    try {
        std::ofstream file(cacheFile);
        file << content;
    } catch (...) {
        // Silently fail if cache can't be written
    }
}

bool NoTorToday::loadCache() {
    try {
        if (!fs::exists(cacheFile)) {
            return false;
        }
        
        std::ifstream file(cacheFile);
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        parseExitAddresses(buffer.str());
        
        struct stat st;
        if (stat(cacheFile.c_str(), &st) == 0) {
            lastUpdate = st.st_mtime;
        }
        
        return true;
    } catch (...) {
        return false;
    }
}
