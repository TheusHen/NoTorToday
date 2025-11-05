import java.io.File
import kotlin.system.measureNanoTime

fun main() {
    val iterations = 1000
    
    // Initialize
    val checker = NoTorToday(autoUpdate = false)
    
    // Load sample data for offline testing
    try {
        val sampleData = File("../../datasets/sample-exit-addresses.txt").readText()
        checker.parseExitAddresses(sampleData)
    } catch (e: Exception) {
        System.err.println("Cannot load data: ${e.message}")
        System.exit(1)
    }
    
    val testIPs = listOf(
        "185.220.101.1",
        "8.8.8.8",
        "1.1.1.1",
        "192.168.1.1",
        "10.0.0.1"
    )
    
    // Warmup
    repeat(100) {
        testIPs.forEach { checker.isTor(it) }
    }
    
    // Benchmark
    val durationNano = measureNanoTime {
        repeat(iterations) {
            testIPs.forEach { checker.isTor(it) }
        }
    }
    
    val durationMs = durationNano / 1_000_000.0
    val opsPerSec = ((iterations * testIPs.size) / (durationMs / 1000.0)).toLong()
    
    // Output JSON
    println("""{"language":"Kotlin","duration_ms":"${"%.2f".format(durationMs)}","operations":${iterations * testIPs.size},"ops_per_second":$opsPerSec,"exit_nodes_loaded":${checker.getCount()}}""")
}
