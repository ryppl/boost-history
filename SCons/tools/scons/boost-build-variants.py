# vi: syntax=python:et:ts=4
from SCons.Script import Split
import traceback

class gcc_features:
    def optimize(self, env, optimize):
        if not optimize or optimize == "no":
            env._AppendFeatureFlag(CCFLAGS = "-O0 -fno-inline")
        elif optimize == "speed":
            env._AppendFeatureFlag(CCFLAGS = "-O3 -finline-functions -Wno-inline")
        elif optimize == "space":
            env._AppendFeatureFlag(CCFLAGS = "-Os")
        else:
            env._AppendFeatureFlag(CCFLAGS = "")

    def profile(self, env, profile):
        if profile:
            env._AppendFeatureFlag(CCFLAGS = "-pg", LINKFLAGS = "-pg")
        else:
            env._AppendFeatureFlag(CCFLAGS = "", LINKFLAGS = "")

    def threading(self, env, threading):
        if threading == "multi":
            env._AppendFeatureFlag(CCFLAGS = "-pthread", LINKFLAGS = "-pthread")
        else:
            env._AppendFeatureFlag(CCFLAGS = "", LINKFLAGS = "")

    def debug(self, env, debug):
        if debug:
            env._AppendFeatureFlag(CCFLAGS = "-g", CPPDEFINES = [])
        else:
            env._AppendFeatureFlag(CCFLAGS = "", CPPDEFINES = "NDEBUG")

def _AppendFeatureFlag(env, **kw):
    stack = traceback.extract_stack(limit = 3)
    feature = stack[0][2].upper()
    for (key, val) in kw.items():
        feature_var = feature + "_" + key
        env.AppendUnique(**{ key : "$" + feature_var })
        env[feature_var] = val

features = gcc_features()

def SetProperty(env, **kw):
    for (prop,value) in kw.items():
        getattr(features, prop, lambda x, y : None)(env, value)
        env[prop.upper()] = value

def boost_suffix(env):
    suffix = str()

    if env["layout"] == "versioned":
        if "gcc" in env["TOOLS"]:
            suffix += "-gcc" + "".join(env["CCVERSION"].split(".")[0:2])

    if env["THREADING"] == "multi":
        suffix += "-mt"
    if env["DEBUG"]:
        suffix += "-d"

    if env["layout"] == "versioned":
        suffix += "-" + "_".join(env["BOOST_VERSION"].split("."))

    return suffix

def exists():
    return True

def generate(env):
    env.AddMethod(SetProperty)
    env.AddMethod(_AppendFeatureFlag)

    env["boost_suffix"] = boost_suffix
    env.Replace(
        CXXFLAGS = Split("-ftemplate-depth-128 -Wall"),
        BOOST_BUILD_DIR = "#/bin.SCons",
        BOOST_SUFFIX = "${boost_suffix(__env__)}"
    )

