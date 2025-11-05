#ifndef NOTORTODAY_H
#define NOTORTODAY_H

#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * NoTorToday - Check if an IP address belongs to a Tor exit node
 */
typedef struct NoTorToday NoTorToday;

/**
 * Create a new NoTorToday instance
 * @param auto_update Whether to automatically download the Tor exit node list
 * @param cache_dir Directory for cache file (NULL for current directory)
 * @return Pointer to NoTorToday instance, or NULL on failure
 */
NoTorToday* notortoday_new(bool auto_update, const char* cache_dir);

/**
 * Free a NoTorToday instance
 * @param nt Pointer to NoTorToday instance
 */
void notortoday_free(NoTorToday* nt);

/**
 * Download and parse the Tor exit node list
 * @param nt Pointer to NoTorToday instance
 * @return 0 on success, -1 on failure
 */
int notortoday_update_list(NoTorToday* nt);

/**
 * Check if an IP address is a Tor exit node
 * @param nt Pointer to NoTorToday instance
 * @param ip IP address to check
 * @return true if the IP is a Tor exit node, false otherwise
 */
bool notortoday_is_tor(NoTorToday* nt, const char* ip);

/**
 * Get the number of loaded Tor exit nodes
 * @param nt Pointer to NoTorToday instance
 * @return The count of exit nodes
 */
int notortoday_get_count(NoTorToday* nt);

/**
 * Get the last update timestamp
 * @param nt Pointer to NoTorToday instance
 * @return The last update time (Unix timestamp), or 0 if never updated
 */
time_t notortoday_get_last_update(NoTorToday* nt);

/**
 * Parse exit addresses from content (for testing with sample data)
 * @param nt Pointer to NoTorToday instance
 * @param content Content to parse
 */
void notortoday_parse_exit_addresses(NoTorToday* nt, const char* content);

#ifdef __cplusplus
}
#endif

#endif /* NOTORTODAY_H */
