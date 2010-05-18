import unittest
import test_mod

class TestVector(unittest.TestCase):

    def testCopyToList(self):
        v = test_mod.return_vector_as_list()
        self.assertEqual(type(v), list)
        self.assertEqual(v, [5,3])

    def testCopyToTuple(self):
        v = test_mod.return_vector_as_tuple()
        self.assertEqual(type(v), tuple)
        self.assertEqual(v, (5,3))

    def testWrapped(self):
        v = test_mod.return_vector_wrapped()
        self.assertEqual(type(v), test_mod.vector)
        self.assert_(test_mod.accept_vector_cref(v))
        self.assert_(test_mod.accept_vector_ref(v))

    def testContainerFromPythonSequence(self):
        v = [5, 3]
        self.assert_(test_mod.accept_vector_cref(v))
        self.assertRaises(TypeError, test_mod.accept_vector_ref, v)

class TestMap(unittest.TestCase):

    def testCopyToDict(self):
        v = test_mod.return_map_as_dict()
        self.assertEqual(type(v), dict)
        self.assertEqual(v, {5:"five",3:"three"})

    def testWrapped(self):
        v = test_mod.return_map_wrapped()
        self.assertEqual(type(v), test_mod.map)
        self.assert_(test_mod.accept_map_cref(v))
        self.assert_(test_mod.accept_map_ref(v))

    def testContainerFromPythonMapping(self):
        v = {5:"five",3:"three"}
        self.assert_(test_mod.accept_map_cref(v))
        self.assertRaises(TypeError, test_mod.accept_map_ref, v)

class TestFusion(unittest.TestCase):

    def testTuple(self):
        v = test_mod.return_tuple()
        self.assertEqual(type(v), tuple)
        self.assertEqual(v, (3, 0.0, "test string"))
        self.assert_(test_mod.accept_tuple(v))

    def testPair(self):
        v = test_mod.return_pair()
        self.assertEqual(type(v), tuple)
        self.assertEqual(v, (3, 5.0))
        self.assert_(test_mod.accept_pair(v))

class TestConstSharedPtr(unittest.TestCase):

    def testSharedPtr(self):
        # only relies on Boost.Python internals, not new code
        v = test_mod.shared_ptr_example_class.return_shared_ptr()
        self.assertEqual(type(v), test_mod.shared_ptr_example_class)

    def testConstSharedPtr(self):
        v = test_mod.shared_ptr_example_class.return_const_shared_ptr()
        self.assertEqual(type(v), test_mod.shared_ptr_example_class)

if __name__=="__main__":
    unittest.main()
