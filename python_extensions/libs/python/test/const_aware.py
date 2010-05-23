import const_aware
import unittest

class TestConstAware(unittest.TestCase):

    def setUp(self):
        self.owner = const_aware.Owner()

    def testByValue(self):
        by_value = self.owner.by_value();
        by_const_value = self.owner.by_const_value();
        self.assertEqual(type(by_value), const_aware.Example)
        self.assertNotEqual(by_value.address, by_const_value.address)
        self.assert_(by_value.const_method())
        self.assert_(by_const_value.const_method())
        self.assert_(by_value.non_const_method())
        self.assert_(by_const_value.non_const_method())

    def testByReference(self):
        by_reference = self.owner.by_reference();
        by_const_reference = self.owner.by_const_reference();
        self.assertEqual(type(by_reference), const_aware.Example)
        self.assertEqual(type(by_const_reference), const_aware.FrozenExample)
        self.assertEqual(by_reference.address, by_const_reference.address)
        self.assert_(by_reference.const_method())
        self.assert_(by_const_reference.const_method())
        self.assert_(by_reference.non_const_method())
        self.assertFalse(hasattr(by_const_reference,"non_const_method"))

if __name__=="__main__":
    unittest.main()
