# vi: syntax=python:et:ts=4
from SCons.Script import AddOption, Flatten

def exists(env):
    return True

def generate(env):
    AddOption('--test', dest='test', action="store_true")

    def BoostCompileTest(env, test, source = None, **kw):
        env.Alias("test", env.Object(test, (source is None) and (test + ".cpp") or source), **kw)
    def BoostRunTest(env, test, source = None, **kw):
        test_prog = env.Program(test, (source is None) and (test + ".cpp") or source, **kw)
        test = env.Command("$BOOST_LIB-"+test, test_prog, "./$SOURCE")
        env.Alias("test", test)
    def BoostRunTests(env, tests, **kw):
        for test in Flatten(tests):
            env.BoostRunTest(test, **kw)
    def BoostCompileTests(env, tests, **kw):
        for test in Flatten(tests):
            env.BoostCompileTest(test, **kw)
    env.AddMethod(BoostCompileTest)
    env.AddMethod(BoostRunTest)
    env.AddMethod(BoostRunTests)
    env.AddMethod(BoostCompileTests)
