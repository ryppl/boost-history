# Copyright David Abrahams & Ralf W. Grosse-Kunsteve 2004-2006.
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from docstring_ext import *

>>> def selected_doc(obj, *args):
...   doc = obj.__doc__.splitlines()
...   return "\\n".join(["|"+doc[i] for i in args])

>>> print selected_doc(X.__init__, 0, 1, 3)
|this is the __init__ function
|its documentation has two lines.
|C++ signature:

>>> print selected_doc(X.value, 0, 2, 4, 5, 7)
|gets the value of the object
|C++ signature:
|
|also gets the value of the object
|C++ signature:

>>> print selected_doc(create, 0, 2)
|creates a new X object
|C++ signature:

>>> print selected_doc(fact, 0, 2)
|compute the factorial
|C++ signature:

>>> len(fact_usr_off_1.__doc__.splitlines())
2
>>> print selected_doc(fact_usr_off_1, 0)
|C++ signature:
>>> len(fact_usr_on_1.__doc__.splitlines())
4
>>> print selected_doc(fact_usr_on_1, 0, 2)
|usr on 1
|C++ signature:
>>> len(fact_usr_off_2.__doc__.splitlines())
2
>>> print selected_doc(fact_usr_off_2, 0)
|C++ signature:
>>> len(fact_usr_on_2.__doc__.splitlines())
4
>>> print selected_doc(fact_usr_on_2, 0, 2)
|usr on 2
|C++ signature:

>>> len(fact_sig_off_1.__doc__.splitlines())
1
>>> print selected_doc(fact_sig_off_1, 0)
|sig off 1
>>> len(fact_sig_on_1.__doc__.splitlines())
4
>>> print selected_doc(fact_sig_on_1, 0, 2)
|sig on 1
|C++ signature:
>>> len(fact_sig_off_2.__doc__.splitlines())
1
>>> print selected_doc(fact_sig_off_2, 0)
|sig off 2
>>> len(fact_sig_on_2.__doc__.splitlines())
4
>>> print selected_doc(fact_sig_on_2, 0, 2)
|sig on 2
|C++ signature:

>>> print fact_usr_off_sig_off_1.__doc__
None
>>> len(fact_usr_on_sig_on_1.__doc__.splitlines())
4
>>> print selected_doc(fact_usr_on_sig_on_1, 0, 2)
|usr on sig on 1
|C++ signature:
>>> len(fact_usr_on_sig_off_1.__doc__.splitlines())
1
>>> print selected_doc(fact_usr_on_sig_off_1, 0)
|usr on sig off 1
>>> len(fact_usr_on_sig_on_2.__doc__.splitlines())
4
>>> print selected_doc(fact_usr_on_sig_on_2, 0, 2)
|usr on sig on 2
|C++ signature:
>>> print fact_usr_off_sig_off_2.__doc__
None

'''

def run(args = None):
    import sys
    import doctest

    if args is not None:
        sys.argv = args

    import docstring_ext

    result = doctest.testmod(sys.modules.get(__name__))

    import pydoc
    import re
    docmodule = lambda m: re.sub(".\10", "", pydoc.text.docmodule(m))
    try:
        print 'printing module help:'
        print docmodule(docstring_ext)
    except object, x:
        print '********* failed **********'
        print x
        result = list(result)
        result[0] += 1
        return tuple(result)

    return result

if __name__ == '__main__':
    print "running..."
    import sys
    status = run()[0]
    if (status == 0): print "Done."
    sys.exit(status)
