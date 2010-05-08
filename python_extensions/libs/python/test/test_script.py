import unittest
import test_mod

class TestVector(unittest.TestCase):

    def testCopyToList(self):
        v = test_mod.return_int_vector_as_list()
        self.assertEqual(type(v), list)
        self.assertEqual(v, [5,3])

    def testWrapped(self):
        v = test_mod.return_int_vector_wrapped()
        self.assertEqual(type(v), test_mod.int_vector)
        self.assert_(test_mod.accept_int_vector_cref(v))
        self.assert_(test_mod.accept_int_vector_ref(v))

    def testContainerFromPythonSequence(self):
        v = [5, 3]
        self.assert_(test_mod.accept_int_vector_cref(v))
        self.assertRaises(TypeError, test_mod.accept_int_vector_ref, v)

if __name__=="__main__":
    unittest.main()
