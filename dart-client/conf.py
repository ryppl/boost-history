prefix = 'prefix'

urls = { 'boost_1_34_0' : 'http://svn.resophonic.com/pub/ctest-tiny' }

build_variants = {
    'debug'  : ['-DBUILD_TESTING:BOOL=ON', '-DBUILD_RELEASE:BOOL=OFF', '-DBUILD_DEBUG:BOOL=ON'],
    'release': ['-DBUILD_TESTING:BOOL=ON', '-DBUILD_RELEASE:BOOL=OFF', '-DBUILD_DEBUG:BOOL=ON']
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
    'nightly' : (['-D', 'Nightly'], minutely_dt)
    }

ctest = "ctest"
cmake = "cmake"

    
