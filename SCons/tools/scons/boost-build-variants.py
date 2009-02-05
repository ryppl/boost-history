# vi: syntax=python:et:ts=4
from SCons.Script import Split

def exists():
    return True

def generate(env):
    env["CXXFLAGS"] = Split("-ftemplate-depth-128 -Wall")
    env["THREADING_MULTI_CCFLAGS"] = "-pthread"
    env["THREADING_MULTI_LINKFLAGS"] = "-pthread"
    env["DEBUG_CCFLAGS"] = Split("-O0 -g -fno-inline")
    env["RELEASE_CCFLAGS"] = Split("-O3 -DNDEBUG -finline-functions -Wno-inline")
