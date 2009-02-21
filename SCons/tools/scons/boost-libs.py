# vi: syntax=python:et:ts=4
import distutils.sysconfig

def BoostLibrary(env, lib, sources):
    if env["LINK_DYNAMIC"]:
        lib_node = env.SharedLibrary("boost_" + lib + env["BOOST_SUFFIX"], sources)
    else:
        lib_node = env.StaticLibrary("boost_" + lib + env["BOOST_SUFFIX"], sources)

    env.Alias(lib, lib_node)
    return lib_node

def BoostUseLib(env, lib):
    env.AppendUnique(
        LIBPATH = env.Dir("#/" + env.Dir(".").path.replace("/" + env["BOOST_LIB"].lower() + "/", "/" + lib + "/")),
        LIBS = ["boost_" + lib + env["BOOST_SUFFIX"]]
    )

def PythonExtension(env, lib, sources, **kw):
    if env["LINK_DYNAMIC"]:
        env.Alias(lib, 
            env.SharedLibrary(lib, sources, SHLIBPREFIX='', SHLIBSUFFIX=distutils.sysconfig.get_config_var("SO"), **kw)
            )

def exists(env):
    return True

def generate(env):
    env.AddMethod(BoostLibrary)
    env.AddMethod(BoostUseLib)
    env.AddMethod(PythonExtension)
