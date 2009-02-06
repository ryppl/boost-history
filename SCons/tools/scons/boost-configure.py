# vi: syntax=python:et:ts=4

def ConfigureBoost(env):
    global conf
    return conf

have_zlib = None
zlib_flags = None
def CheckZLib(env):
    global have_zlib, zlib_flags
    if zlib_flags == None:
        zlib_flags = dict(LIBS = ["z"])
        conf = env.ConfigureBoost()
        have_zlib = conf.CheckLibWithHeader("z", "zlib.h", "c", autoadd = False)
    if have_zlib:
        env.AppendUnique(**zlib_flags)
        return have_zlib

have_bzip2 = None
bzip2_flags = None
def CheckBZip2(env):
    global have_bzip2, bzip2_flags
    if bzip2_flags == None:
        bzip2_flags = dict(LIBS = ["bz2"])
        conf = env.ConfigureBoost()
        have_bzip2 = conf.CheckLibWithHeader("bz2", "bzlib.h", "c", autoadd = False)
    if have_bzip2:
        env.AppendUnique(**bzip2_flags)
        return have_bzip2

def generate(env):
    env.AddMethod(ConfigureBoost)
    env.AddMethod(CheckZLib)
    env.AddMethod(CheckBZip2)
    global conf
    build_dir = "#/bin.SCons/"
    conf = env.Configure(conf_dir = build_dir + "sconf_temp", log_file = build_dir + "config.log")

def exists():
    return True
