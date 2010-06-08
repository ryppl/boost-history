import const_aware_mod
import unittest
import pickle

const_aware_mod.Example.__reduce__ = lambda self: (const_aware_mod.Example, (self.value_prop,))

class TestConstAware(unittest.TestCase):

    def setUp(self):
        self.owner = const_aware_mod.Owner()

    def checkNonConst(self, x):
        self.assertEqual(type(x), const_aware_mod.Example)        
        self.assert_(x.const_method())
        self.assert_(x.non_const_method())
        self.assert_(self.owner.accept_by_value(x))
        self.assert_(self.owner.accept_by_const_value(x))
        self.assert_(self.owner.accept_by_reference(x))
        self.assert_(self.owner.accept_by_const_reference(x))
        self.assert_(self.owner.accept_by_shared_ptr(x))
        self.assert_(self.owner.accept_by_const_shared_ptr(x))
        x.value_prop = 0
        self.assertEqual(x.value_prop, 0)
        self.assertEqual(x.value_ro, 0)
        self.assertEqual(x.value_rw, 0)
        x.value_prop = 1
        self.assertEqual(x.value_prop, 1)
        self.assertEqual(x.value_ro, 1)
        self.assertEqual(x.value_rw, 1)
        x.value_rw = 0
        self.assertEqual(x.value_prop, 0)
        self.assertEqual(x.value_ro, 0)
        self.assertEqual(x.value_rw, 0)
        def set_value_ro(v):
            x.value_ro = v
        self.assertRaises(AttributeError, set_value_ro, 1)

    def checkConst(self, x):
        self.assertEqual(type(x), const_aware_mod.Example.__const_proxy__)
        self.assert_(x.const_method())
        self.assertFalse(hasattr(x,"non_const_method"))
        self.assert_(self.owner.accept_by_value(x))
        self.assert_(self.owner.accept_by_const_value(x))
        self.assertRaises(Exception, self.owner.accept_by_reference, x)
        self.assert_(self.owner.accept_by_const_reference(x))
        self.assertRaises(Exception, self.owner.accept_by_shared_ptr, x)
        self.assert_(self.owner.accept_by_const_shared_ptr(x))
        self.assertEqual(x.value_prop, 0)
        self.assertEqual(x.value_ro, 0)
        self.assertEqual(x.value_rw, 0)
        def set_value_prop(v):
            x.value_prop = v
        def set_value_rw(v):
            x.value_rw = v
        def set_value_ro(v):
            x.value_ro = v
        self.assertRaises(AttributeError, set_value_prop, 1)
        self.assertRaises(AttributeError, set_value_rw, 1)
        self.assertRaises(AttributeError, set_value_ro, 1)

    def testByValue(self):
        by_value = self.owner.by_value()
        by_const_value = self.owner.by_const_value()
        self.assertNotEqual(by_value.address, by_const_value.address)
        self.checkNonConst(by_value)
        self.checkConst(by_const_value)

    def testByReference(self):
        by_reference = self.owner.by_reference()
        by_const_reference = self.owner.by_const_reference()
        self.assertEqual(by_reference.address, by_const_reference.address)
        self.checkNonConst(by_reference)
        self.checkConst(by_const_reference)

    def testBySharedPtr(self):
        by_shared_ptr = self.owner.by_shared_ptr();
        by_const_shared_ptr = self.owner.by_const_shared_ptr();
        self.assertEqual(by_shared_ptr.address, by_const_shared_ptr.address)
        self.checkNonConst(by_shared_ptr)
        self.checkConst(by_const_shared_ptr)

    def testConstruction(self):
        original = self.owner.by_value()
        proxy = const_aware_mod.Example.__const_proxy__(original)
        self.assertEqual(original.address, original.address)
        self.checkConst(proxy)
        original_copy = const_aware_mod.Example(original)
        proxy_copy = const_aware_mod.Example(proxy)
        self.assertNotEqual(original.address, original_copy.address)
        self.assertNotEqual(proxy.address, proxy_copy.address)
        self.checkNonConst(original_copy)
        self.checkNonConst(proxy_copy)

    def testPickle(self):
        original = self.owner.by_value()
        original.value_prop = 3
        original_pickled = pickle.dumps(original)
        original_loaded = pickle.loads(original_pickled)
        self.assertEqual(original_loaded.value_prop, original.value_prop)
        proxy = const_aware_mod.Example.__const_proxy__(original)
        proxy_pickled = pickle.dumps(proxy)
        proxy_loaded = pickle.loads(proxy_pickled)
        self.assertEqual(proxy_loaded.value_prop, proxy.value_prop)
        self.checkNonConst(proxy_loaded)

if __name__=="__main__":
    unittest.main()
