# vi: syntax=python:et:ts=4
from SCons.Script import Split

class gcc_features:
    def optimize(self, env, optimize):
        env.AppendUnique(CCFLAGS = "$OPTIMIZE_CCFLAGS")
        if not optimize or optimize == "no":
            env.Replace(OPTIMIZE_CCFLAGS = "-O0")
        elif optimize == "speed":
            env.Replace(OPTIMIZE_CCFLAGS = "-O3 -finline-functions -Wno-inline")
        elif optimize == "space":
            env.Replace(OPTIMIZE_CCFLAGS = "-Os")
        else:
            env.Replace(OPTIMIZE_CCFLAGS = "")

    def profile(self, env, profile):
        env.AppendUnique(CCFLAGS = "$PROFILE_CCFLAGS", LINKFLAGS = "$PROFILE_LINKFLAGS")
        if profile:
            env.Replace(PROFILE_CCFLAGS = "-pg", PROFILE_LINKFLAGS = "-pg")
        else:
            env.Replace(PROFILE_CCFLAGS = "", PROFILE_LINKFLAGS = "")

features = gcc_features()

def SetProperty(env, **kw):
    for (prop,value) in kw.items():
        getattr(features, prop, lambda x, y : None)(env, value)

def exists():
    return True

def generate(env):
    env["CXXFLAGS"] = Split("-ftemplate-depth-128 -Wall")
    env.AddMethod(SetProperty)
    env["THREADING_MULTI_CCFLAGS"] = "-pthread"
    env["THREADING_MULTI_LINKFLAGS"] = "-pthread"
    env["DEBUG_CCFLAGS"] = Split("-g -fno-inline")
    env["RELEASE_CCFLAGS"] = Split("-DNDEBUG")
