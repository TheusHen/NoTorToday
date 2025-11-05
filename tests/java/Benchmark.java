import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

public class Benchmark {
    static class BenchmarkResult {
        String language;
        String duration_ms;
        int operations;
        long ops_per_second;
        int exit_nodes_loaded;
    }
    
    public static void main(String[] args) {
        int iterations = 1000;
        
        // Initialize
        NoTorToday checker = new NoTorToday(false);
        
        // Load sample data for offline testing
        try {
            String sampleData = new String(
                Files.readAllBytes(Paths.get("../../datasets/sample-exit-addresses.txt"))
            );
            checker.parseExitAddresses(sampleData);
        } catch (IOException e) {
            System.err.println("Cannot load data: " + e.getMessage());
            System.exit(1);
        }
        
        List<String> testIPs = Arrays.asList(
            "185.220.101.1",
            "8.8.8.8",
            "1.1.1.1",
            "192.168.1.1",
            "10.0.0.1"
        );
        
        // Warmup
        for (int i = 0; i < 100; i++) {
            for (String ip : testIPs) {
                checker.isTor(ip);
            }
        }
        
        // Benchmark
        long startTime = System.nanoTime();
        
        for (int i = 0; i < iterations; i++) {
            for (String ip : testIPs) {
                checker.isTor(ip);
            }
        }
        
        long endTime = System.nanoTime();
        double durationMs = (endTime - startTime) / 1_000_000.0;
        long opsPerSec = (long)((iterations * testIPs.size()) / (durationMs / 1000.0));
        
        // Output JSON
        Map<String, Object> result = new HashMap<>();
        result.put("language", "Java");
        result.put("duration_ms", String.format("%.2f", durationMs));
        result.put("operations", iterations * testIPs.size());
        result.put("ops_per_second", opsPerSec);
        result.put("exit_nodes_loaded", checker.getCount());
        
        // Simple JSON output without Gson dependency
        System.out.println("{" +
            "\"language\":\"Java\"," +
            "\"duration_ms\":\"" + String.format("%.2f", durationMs) + "\"," +
            "\"operations\":" + (iterations * testIPs.size()) + "," +
            "\"ops_per_second\":" + opsPerSec + "," +
            "\"exit_nodes_loaded\":" + checker.getCount() +
            "}");
    }
}
