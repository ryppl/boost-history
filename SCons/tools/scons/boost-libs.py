# vi: syntax=python:et:ts=4

def BoostLibrary(env, lib, sources):
    if env["LINK_DYNAMIC"]:
        lib_node = env.SharedLibrary("boost_" + lib, sources)
    else:
        lib_node = env.StaticLibrary("boost_" + lib, sources)

    env.Alias(lib, lib_node)
    return lib_node

def exists(env):
    return True

def generate(env):
    env.AddMethod(BoostLibrary)
