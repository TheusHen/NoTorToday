import sys
import os
import unittest

# Add core directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../core/python'))

from notortoday import NoTorToday

class TestNoTorToday(unittest.TestCase):
    def setUp(self):
        """Load sample data for tests"""
        sample_path = os.path.join(os.path.dirname(__file__), 
                                   '../../datasets/sample-exit-addresses.txt')
        with open(sample_path, 'r') as f:
            self.sample_data = f.read()
    
    def test_parse_exit_addresses(self):
        """Test parsing exit addresses"""
        checker = NoTorToday(auto_update=False)
        checker.parse_exit_addresses(self.sample_data)
        self.assertEqual(checker.get_count(), 10, 'Should load 10 exit nodes')
    
    def test_identify_tor_ips(self):
        """Test identifying Tor IPs"""
        checker = NoTorToday(auto_update=False)
        checker.parse_exit_addresses(self.sample_data)
        
        self.assertTrue(checker.is_tor('185.220.101.1'), '185.220.101.1 should be Tor')
        self.assertTrue(checker.is_tor('185.220.101.2'), '185.220.101.2 should be Tor')
        self.assertFalse(checker.is_tor('8.8.8.8'), '8.8.8.8 should not be Tor')
        self.assertFalse(checker.is_tor('1.1.1.1'), '1.1.1.1 should not be Tor')
    
    def test_validate_ipv4(self):
        """Test IPv4 validation"""
        checker = NoTorToday(auto_update=False)
        
        self.assertTrue(checker.is_valid_ip('192.168.1.1'))
        self.assertTrue(checker.is_valid_ip('255.255.255.255'))
        self.assertTrue(checker.is_valid_ip('0.0.0.0'))
        self.assertFalse(checker.is_valid_ip('256.1.1.1'))
        self.assertFalse(checker.is_valid_ip('invalid'))
        self.assertFalse(checker.is_valid_ip('192.168.1'))
    
    def test_validate_ipv6(self):
        """Test IPv6 validation"""
        checker = NoTorToday(auto_update=False)
        
        self.assertTrue(checker.is_valid_ip('2001:0db8:85a3:0000:0000:8a2e:0370:7334'))
        self.assertTrue(checker.is_valid_ip('::1'))
        self.assertFalse(checker.is_valid_ip('invalid:ipv6'))
    
    def test_cache_operations(self):
        """Test cache save and load"""
        checker = NoTorToday(auto_update=False)
        checker.parse_exit_addresses(self.sample_data)
        checker.save_cache(self.sample_data)
        
        checker2 = NoTorToday(auto_update=False)
        loaded = checker2.load_cache()
        
        self.assertTrue(loaded, 'Should load cache successfully')
        self.assertEqual(checker2.get_count(), 10, 'Should have same count after loading cache')

if __name__ == '__main__':
    print('Running NoTorToday Python Tests\n')
    unittest.main(verbosity=2)
