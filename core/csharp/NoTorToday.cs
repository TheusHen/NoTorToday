using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Net.Sockets;
using System.Threading.Tasks;

/// <summary>
/// NoTorToday - Check if an IP address belongs to a Tor exit node
/// 
/// Usage:
///   var checker = new NoTorToday(autoUpdate: true);
///   await checker.UpdateListAsync();
///   bool isTor = checker.IsTor("185.220.101.1");
/// </summary>
public class NoTorToday
{
    private HashSet<string> data;
    private DateTime? lastUpdate;
    private string cacheFile;
    private const string TorListUrl = "https://check.torproject.org/exit-addresses";
    private static readonly HttpClient httpClient = new HttpClient { Timeout = TimeSpan.FromSeconds(10) };

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="autoUpdate">Whether to automatically download the Tor exit node list</param>
    /// <param name="cacheDir">Directory for cache file (null for current directory)</param>
    public NoTorToday(bool autoUpdate = true, string cacheDir = null)
    {
        data = new HashSet<string>();
        lastUpdate = null;

        string dir = cacheDir ?? Directory.GetCurrentDirectory();
        cacheFile = Path.Combine(dir, ".notortoday_cache");

        if (autoUpdate)
        {
            try
            {
                UpdateListAsync().Wait();
            }
            catch
            {
                // Try to load from cache if update fails
                LoadCache();
            }
        }
        else
        {
            LoadCache();
        }
    }

    /// <summary>
    /// Download and parse the Tor exit node list
    /// </summary>
    public async Task UpdateListAsync()
    {
        string content = await DownloadTorListAsync();
        ParseExitAddresses(content);
        SaveCache(content);
        lastUpdate = DateTime.UtcNow;
    }

    /// <summary>
    /// Download Tor exit node list from torproject.org
    /// </summary>
    private async Task<string> DownloadTorListAsync()
    {
        var response = await httpClient.GetAsync(TorListUrl);
        response.EnsureSuccessStatusCode();
        return await response.Content.ReadAsStringAsync();
    }

    /// <summary>
    /// Parse exit addresses from the downloaded data
    /// </summary>
    public void ParseExitAddresses(string content)
    {
        data.Clear();
        string[] lines = content.Split(new[] { '\n', '\r' }, StringSplitOptions.RemoveEmptyEntries);

        foreach (string line in lines)
        {
            if (line.StartsWith("ExitAddress "))
            {
                string[] parts = line.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length >= 2)
                {
                    string ip = parts[1].Trim();
                    if (IsValidIP(ip))
                    {
                        data.Add(ip);
                    }
                }
            }
        }
    }

    /// <summary>
    /// Validate IP address (IPv4 and IPv6)
    /// </summary>
    private bool IsValidIP(string ip)
    {
        return IPAddress.TryParse(ip, out _);
    }

    /// <summary>
    /// Check if an IP address is a Tor exit node
    /// </summary>
    public bool IsTor(string ip)
    {
        return data.Contains(ip);
    }

    /// <summary>
    /// Get the number of loaded Tor exit nodes
    /// </summary>
    public int GetCount()
    {
        return data.Count;
    }

    /// <summary>
    /// Get the last update timestamp
    /// </summary>
    public DateTime? GetLastUpdate()
    {
        return lastUpdate;
    }

    /// <summary>
    /// Save cache to disk
    /// </summary>
    private void SaveCache(string content)
    {
        try
        {
            File.WriteAllText(cacheFile, content);
        }
        catch
        {
            // Silently fail if cache can't be written
        }
    }

    /// <summary>
    /// Load cache from disk
    /// </summary>
    private bool LoadCache()
    {
        try
        {
            if (File.Exists(cacheFile))
            {
                string content = File.ReadAllText(cacheFile);
                ParseExitAddresses(content);
                lastUpdate = File.GetLastWriteTimeUtc(cacheFile);
                return true;
            }
        }
        catch
        {
            // Silently fail if cache can't be read
        }
        return false;
    }
}
