import 'dart:io';
import 'dart:convert';

/// NoTorToday - Check if an IP address belongs to a Tor exit node
/// 
/// Usage:
///   var checker = NoTorToday(autoUpdate: true);
///   await checker.updateList();
///   bool isTor = checker.isTor('185.220.101.1');
class NoTorToday {
  final Set<String> _data = {};
  DateTime? _lastUpdate;
  late String _cacheFile;
  static const String _torListUrl = 'https://check.torproject.org/exit-addresses';

  /// Constructor
  /// 
  /// [autoUpdate] Whether to automatically download the Tor exit node list
  /// [cacheDir] Directory for cache file (null for current directory)
  NoTorToday({bool autoUpdate = true, String? cacheDir}) {
    final dir = cacheDir ?? Directory.current.path;
    _cacheFile = '$dir/.notortoday_cache';

    if (autoUpdate) {
      updateList().catchError((e) {
        // Try to load from cache if update fails
        if (!_loadCache()) {
          print('Failed to update and no cache available: $e');
        }
      });
    } else {
      _loadCache();
    }
  }

  /// Download and parse the Tor exit node list
  Future<void> updateList() async {
    final content = await _downloadTorList();
    parseExitAddresses(content);
    await _saveCache(content);
    _lastUpdate = DateTime.now();
  }

  /// Download Tor exit node list from torproject.org
  Future<String> _downloadTorList() async {
    final client = HttpClient();
    try {
      final request = await client.getUrl(Uri.parse(_torListUrl));
      final response = await request.close();
      
      if (response.statusCode != 200) {
        throw Exception('HTTP ${response.statusCode}');
      }
      
      return await response.transform(utf8.decoder).join();
    } finally {
      client.close();
    }
  }

  /// Parse exit addresses from the downloaded data
  void parseExitAddresses(String content) {
    _data.clear();
    final lines = content.split('\n');

    for (final line in lines) {
      if (line.startsWith('ExitAddress ')) {
        final parts = line.split(RegExp(r'\s+'));
        if (parts.length >= 2) {
          final ip = parts[1].trim();
          if (_isValidIP(ip)) {
            _data.add(ip);
          }
        }
      }
    }
  }

  /// Validate IP address (IPv4 and IPv6)
  bool _isValidIP(String ip) {
    try {
      InternetAddress(ip);
      return true;
    } catch (e) {
      return false;
    }
  }

  /// Check if an IP address is a Tor exit node
  bool isTor(String ip) {
    return _data.contains(ip);
  }

  /// Get the number of loaded Tor exit nodes
  int getCount() {
    return _data.length;
  }

  /// Get the last update timestamp
  DateTime? getLastUpdate() {
    return _lastUpdate;
  }

  /// Save cache to disk
  Future<void> _saveCache(String content) async {
    try {
      await File(_cacheFile).writeAsString(content);
    } catch (e) {
      // Silently fail if cache can't be written
    }
  }

  /// Load cache from disk
  bool _loadCache() {
    try {
      final file = File(_cacheFile);
      if (file.existsSync()) {
        final content = file.readAsStringSync();
        parseExitAddresses(content);
        _lastUpdate = file.lastModifiedSync();
        return true;
      }
    } catch (e) {
      // Silently fail if cache can't be read
    }
    return false;
  }
}
