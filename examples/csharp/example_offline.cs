using System;
using System.IO;

class ExampleOffline
{
    static void Main(string[] args)
    {
        Console.WriteLine("NoTorToday - C# Example (Offline Mode)\n");
        
        // Create instance without auto-update
        var checker = new NoTorToday(autoUpdate: false);
        
        // Load sample data for testing
        try
        {
            string sampleData = File.ReadAllText("../../datasets/sample-exit-addresses.txt");
            checker.ParseExitAddresses(sampleData);
            
            Console.WriteLine($"Loaded {checker.GetCount()} Tor exit nodes");
            Console.WriteLine("Using sample dataset for offline testing\n");
            
            // Test IPs
            string[] testIPs = {
                "185.220.101.1",  // In sample dataset - should be Tor
                "185.220.101.2",  // In sample dataset - should be Tor
                "8.8.8.8",        // Google DNS - not Tor
                "1.1.1.1"         // Cloudflare DNS - not Tor
            };
            
            Console.WriteLine("Testing IP addresses:");
            foreach (var ip in testIPs)
            {
                bool isTor = checker.IsTor(ip);
                Console.WriteLine($"  {ip}: {(isTor ? "TOR EXIT NODE ✓" : "Not Tor ✗")}");
            }
        }
        catch (Exception e)
        {
            Console.Error.WriteLine($"Error: {e.Message}");
        }
    }
}
