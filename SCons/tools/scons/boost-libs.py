# vi: syntax=python:et:ts=4

def BoostLibrary(env, lib, sources):
    env.StaticLibrary("boost_" + lib, sources)

def exists(env):
    return True

def generate(env):
    env.AddMethod(BoostLibrary)
