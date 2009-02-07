# vi: syntax=python:et:ts=4

class DependencyChecker:
    flags = {}
    checked = False
    have_dep = False
    def __init__(self):
        self.__name__ = str(self.__class__).split(".")[-1]
    def __call__(self, env):
        if not self.checked:
            build_dir = "#/bin.SCons/"
            self.conf = env.Configure(conf_dir = build_dir + "sconf_temp", log_file = build_dir + "config.log")
            self.Check(env)
            self.conf.Finish()
            self.checked = True
        if(self.have_dep):
            env.AppendUnique(**self.flags)
            return True
        else:
            return False

class CheckZLib(DependencyChecker):
    def Check(self, env):
        self.flags = dict(LIBS = ["z"])
        self.have_dep = self.conf.CheckLibWithHeader("z", "zlib.h", "c", autoadd = False)

class CheckBZip2(DependencyChecker):
    def Check(self, env):
        self.flags = dict(LIBS = ["bz2"])
        self.have_dep = self.conf.CheckLibWithHeader("bz2", "bzlib.h", "c", autoadd = False)

def generate(env):
    env.AddMethod(CheckZLib())
    env.AddMethod(CheckBZip2())

def exists():
    return True
