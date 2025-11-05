#include "notortoday.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>

#define TOR_LIST_URL "https://check.torproject.org/exit-addresses"
#define MAX_IPS 10000
#define MAX_IP_LEN 46  // IPv6 max length

struct NoTorToday {
    char** ips;
    int count;
    int capacity;
    time_t last_update;
    char cache_file[512];
};

// Helper for HTTP download
struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;
    
    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) return 0;
    
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

static bool is_valid_ipv4(const char* ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1;
}

static bool is_valid_ipv6(const char* ip) {
    struct sockaddr_in6 sa;
    return inet_pton(AF_INET6, ip, &(sa.sin6_addr)) == 1;
}

static bool is_valid_ip(const char* ip) {
    return is_valid_ipv4(ip) || is_valid_ipv6(ip);
}

static void add_ip(NoTorToday* nt, const char* ip) {
    if (nt->count >= nt->capacity) {
        nt->capacity *= 2;
        nt->ips = realloc(nt->ips, nt->capacity * sizeof(char*));
    }
    nt->ips[nt->count] = strdup(ip);
    nt->count++;
}

void notortoday_parse_exit_addresses(NoTorToday* nt, const char* content) {
    // Free existing IPs
    for (int i = 0; i < nt->count; i++) {
        free(nt->ips[i]);
    }
    nt->count = 0;
    
    char* content_copy = strdup(content);
    char* line = strtok(content_copy, "\n");
    
    while (line != NULL) {
        if (strncmp(line, "ExitAddress ", 12) == 0) {
            char ip[MAX_IP_LEN];
            if (sscanf(line, "ExitAddress %45s", ip) == 1) {
                if (is_valid_ip(ip)) {
                    add_ip(nt, ip);
                }
            }
        }
        line = strtok(NULL, "\n");
    }
    
    free(content_copy);
}

static int download_tor_list(NoTorToday* nt, char** content) {
    CURL* curl;
    CURLcode res;
    struct MemoryStruct chunk = {0};
    
    chunk.memory = malloc(1);
    chunk.size = 0;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (!curl) {
        free(chunk.memory);
        return -1;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, TOR_LIST_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    if (res != CURLE_OK) {
        free(chunk.memory);
        return -1;
    }
    
    *content = chunk.memory;
    return 0;
}

static void save_cache(NoTorToday* nt, const char* content) {
    FILE* fp = fopen(nt->cache_file, "w");
    if (fp) {
        fputs(content, fp);
        fclose(fp);
    }
}

static bool load_cache(NoTorToday* nt) {
    FILE* fp = fopen(nt->cache_file, "r");
    if (!fp) return false;
    
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* content = malloc(fsize + 1);
    fread(content, 1, fsize, fp);
    content[fsize] = 0;
    fclose(fp);
    
    notortoday_parse_exit_addresses(nt, content);
    
    struct stat st;
    if (stat(nt->cache_file, &st) == 0) {
        nt->last_update = st.st_mtime;
    }
    
    free(content);
    return true;
}

NoTorToday* notortoday_new(bool auto_update, const char* cache_dir) {
    NoTorToday* nt = malloc(sizeof(NoTorToday));
    if (!nt) return NULL;
    
    nt->capacity = 1000;
    nt->ips = malloc(nt->capacity * sizeof(char*));
    nt->count = 0;
    nt->last_update = 0;
    
    const char* dir = cache_dir ? cache_dir : ".";
    snprintf(nt->cache_file, sizeof(nt->cache_file), "%s/.notortoday_cache", dir);
    
    if (auto_update) {
        if (notortoday_update_list(nt) != 0) {
            load_cache(nt);
        }
    } else {
        load_cache(nt);
    }
    
    return nt;
}

void notortoday_free(NoTorToday* nt) {
    if (!nt) return;
    
    for (int i = 0; i < nt->count; i++) {
        free(nt->ips[i]);
    }
    free(nt->ips);
    free(nt);
}

int notortoday_update_list(NoTorToday* nt) {
    char* content = NULL;
    
    if (download_tor_list(nt, &content) != 0) {
        return -1;
    }
    
    notortoday_parse_exit_addresses(nt, content);
    save_cache(nt, content);
    nt->last_update = time(NULL);
    
    free(content);
    return 0;
}

bool notortoday_is_tor(NoTorToday* nt, const char* ip) {
    for (int i = 0; i < nt->count; i++) {
        if (strcmp(nt->ips[i], ip) == 0) {
            return true;
        }
    }
    return false;
}

int notortoday_get_count(NoTorToday* nt) {
    return nt->count;
}

time_t notortoday_get_last_update(NoTorToday* nt) {
    return nt->last_update;
}
