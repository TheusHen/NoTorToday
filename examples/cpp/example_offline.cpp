#include "../../core/cpp/notortoday.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    std::cout << "NoTorToday - C++ Example (Offline Mode)\n\n";
    
    // Create instance without auto-update
    NoTorToday checker(false);
    
    // Load sample data for testing
    try {
        std::string sampleData = readFile("../../datasets/sample-exit-addresses.txt");
        checker.parseExitAddresses(sampleData);
        
        std::cout << "Loaded " << checker.getCount() << " Tor exit nodes\n";
        std::cout << "Using sample dataset for offline testing\n\n";
        
        // Test IPs
        const char* testIPs[] = {
            "185.220.101.1",  // In sample dataset - should be Tor
            "185.220.101.2",  // In sample dataset - should be Tor
            "8.8.8.8",        // Google DNS - not Tor
            "1.1.1.1"         // Cloudflare DNS - not Tor
        };
        
        std::cout << "Testing IP addresses:\n";
        for (const auto& ip : testIPs) {
            bool isTor = checker.isTor(ip);
            std::cout << "  " << ip << ": " << (isTor ? "TOR EXIT NODE ✓" : "Not Tor ✗") << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
