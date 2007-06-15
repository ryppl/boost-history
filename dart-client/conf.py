prefix = 'prefix'

boost_svn = 'http://svn.boost.org/svn/boost/sandbox-branches/boost-cmake/boost_1_34_0'
test_svn = 'http://svn.resophonic.com/pub/ctest-tiny'
urls = { 'boost_1_34_0' : boost_svn }

build_variants = {
    'debug'  : ['-DBUILD_TESTING:BOOL=ON', '-DBUILD_RELEASE:BOOL=OFF', '-DBUILD_DEBUG:BOOL=ON'],
    'release': ['-DBUILD_TESTING:BOOL=ON', '-DBUILD_RELEASE:BOOL=ON', '-DBUILD_DEBUG:BOOL=OFF']
}

def nightly_dt(t):
    current_time = datetime.now()
    if current_time.date() > t.date():
        return current_time - t
    else:
        return timedelta(-1)  # delta of negative one day, won't get run

def minutely_dt(t):
    current_time = datetime.now()
    if current_time.replace(microsecond=0, second=0) > t:
        return current_time - t
    else:
        return timedelta(-1)  # delta of negative one day, won't get run

def continuous_dt(t):
    current_time = datetime.now()
    return current_time - t

ctest_variants = { 
    'continuous' : (['-D', 'Continuous'], continuous_dt),
    'nightly' : (['-D', 'Nightly'], nightly_dt)
    }

ctest = "ctest"
cmake = "cmake"

    
