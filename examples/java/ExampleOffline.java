import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class ExampleOffline {
    public static void main(String[] args) {
        System.out.println("NoTorToday - Java Example (Offline Mode)\n");
        
        // Create instance without auto-update
        NoTorToday checker = new NoTorToday(false);
        
        // Load sample data for testing
        try {
            String sampleData = new String(
                Files.readAllBytes(Paths.get("../../datasets/sample-exit-addresses.txt"))
            );
            checker.parseExitAddresses(sampleData);
            
            System.out.println("Loaded " + checker.getCount() + " Tor exit nodes");
            System.out.println("Using sample dataset for offline testing\n");
            
            // Test IPs
            String[] testIPs = {
                "185.220.101.1",  // In sample dataset - should be Tor
                "185.220.101.2",  // In sample dataset - should be Tor
                "8.8.8.8",        // Google DNS - not Tor
                "1.1.1.1"         // Cloudflare DNS - not Tor
            };
            
            System.out.println("Testing IP addresses:");
            for (String ip : testIPs) {
                boolean isTor = checker.isTor(ip);
                System.out.println("  " + ip + ": " + (isTor ? "TOR EXIT NODE ✓" : "Not Tor ✗"));
            }
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
