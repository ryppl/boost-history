# vi: syntax=python:et:ts=4
from subprocess import Popen, PIPE

class DependencyChecker:
    flags = {}
    checked = False
    have_dep = False
    def __init__(self):
        self.__name__ = str(self.__class__).split(".")[-1]
    def __call__(self, env):
        if not self.checked:
            conf_env = env.Clone()
            build_dir = "$BOOST_BUILD_DIR/"
            self.conf = conf_env.Configure(conf_dir = build_dir + "sconf_temp", log_file = build_dir + "config.log")
            self.Check(conf_env)
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

class CheckPython(DependencyChecker):
    def Check(self, env):
        import distutils.sysconfig
        self.flags = dict(
            CPPPATH = [distutils.sysconfig.get_python_inc()],
            LIBPATH = [distutils.sysconfig.PREFIX + "/lib"],
            LIBS = "python" + distutils.sysconfig.get_config_var('VERSION'))
        env.AppendUnique(**self.flags)
        self.have_dep = self.conf.CheckCHeader("Python.h")

class CheckMPI(DependencyChecker):
    def Check(self, env):
        try:
            self.flags = env.ParseFlags(Popen(["mpic++", "-showme"], stdout = PIPE).communicate()[0].split(" ", 1)[1])
            env.AppendUnique(**self.flags)
            self.have_dep = self.conf.CheckCHeader("mpi.h")
        except:
            self.have_dep = False

class CheckICU(DependencyChecker):
    def Check(self, env):
        try:
            self.flags = env.ParseFlags(Popen(["icu-config", "--ldflags", "--cxxflags"], stdout = PIPE).communicate()[0])
            env.AppendUnique(**self.flags)
            self.have_dep = self.conf.CheckCHeader("unicode/utypes.h")
        except:
            self.have_dep = False

class CheckExpat(DependencyChecker):
    def Check(self, env):
        try:
            self.flags = dict(LIBS = ["expat"])
            env.AppendUnique(**self.flags)
            self.have_dep = self.conf.CheckLibWithHeader("expat", "expat.h", "c", autoadd = False)
        except:
            self.have_dep = False

def generate(env):
    env.AddMethod(CheckZLib())
    env.AddMethod(CheckBZip2())
    env.AddMethod(CheckPython())
    env.AddMethod(CheckMPI())
    env.AddMethod(CheckICU())
    env.AddMethod(CheckExpat())

def exists():
    return True
