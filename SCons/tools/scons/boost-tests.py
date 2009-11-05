# vi: syntax=python:et:ts=4
from SCons.Script import AddOption, Flatten

def exists(env):
    return True

def generate(env):
    AddOption('--test', dest='test', action="store_true")

    def BoostCompileTest(env, test, source = None, **kw):
        env.Alias("$BOOST_LIB", env.Object(test, (source is None) and (test + ".cpp") or source), **kw)
    def BoostRun(env, prog, name, command = "./$SOURCE"):
        run = env.Command("$BOOST_LIB-"+name, prog, command)
        env.Alias("$BOOST_LIB", run)
        return run
    def BoostRunPythonScript(env, script):
        return env.BoostRun(env.File(script), "run-" + script, "python $SOURCE")
    def BoostRunTest(env, test, source = None, command = "./$SOURCE", command_sources = [], **kw):
        test_prog = env.Program(test, (source is None) and (test + ".cpp") or source, **kw)
        return env.BoostRun([test_prog, command_sources], test, command)
    def BoostRunTests(env, tests, **kw):
        for test in Flatten(tests):
            env.BoostRunTest(test, **kw)
    def BoostCompileTests(env, tests, **kw):
        for test in Flatten(tests):
            env.BoostCompileTest(test, **kw)
    env.AddMethod(BoostCompileTest)
    env.AddMethod(BoostRun)
    env.AddMethod(BoostRunPythonScript)
    env.AddMethod(BoostRunTest)
    env.AddMethod(BoostRunTests)
    env.AddMethod(BoostCompileTests)
