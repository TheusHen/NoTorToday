#include <stdio.h>
#include <stdlib.h>
#include "../../core/c/notortoday.h"

static char* read_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;
    
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* content = malloc(fsize + 1);
    fread(content, 1, fsize, fp);
    content[fsize] = 0;
    fclose(fp);
    
    return content;
}

int main() {
    printf("NoTorToday - C Example (Offline Mode)\n\n");
    
    // Create instance without auto-update
    NoTorToday* checker = notortoday_new(false, NULL);
    if (!checker) {
        fprintf(stderr, "Failed to create NoTorToday instance\n");
        return 1;
    }
    
    // Load sample data for testing
    char* sampleData = read_file("../../datasets/sample-exit-addresses.txt");
    if (!sampleData) {
        fprintf(stderr, "Failed to load sample data\n");
        notortoday_free(checker);
        return 1;
    }
    
    notortoday_parse_exit_addresses(checker, sampleData);
    free(sampleData);
    
    printf("Loaded %d Tor exit nodes\n", notortoday_get_count(checker));
    printf("Using sample dataset for offline testing\n\n");
    
    // Test IPs
    const char* testIPs[] = {
        "185.220.101.1",  // In sample dataset - should be Tor
        "185.220.101.2",  // In sample dataset - should be Tor
        "8.8.8.8",        // Google DNS - not Tor
        "1.1.1.1"         // Cloudflare DNS - not Tor
    };
    
    printf("Testing IP addresses:\n");
    for (int i = 0; i < 4; i++) {
        bool isTor = notortoday_is_tor(checker, testIPs[i]);
        printf("  %s: %s\n", testIPs[i], isTor ? "TOR EXIT NODE ✓" : "Not Tor ✗");
    }
    
    notortoday_free(checker);
    return 0;
}
