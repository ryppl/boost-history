#!/usr/bin/python
import os

# clear environment for testing
#
for s in (
    'BOOST_ROOT','BOOST_BUILD_PATH','JAM_TOOLSET','BCCROOT',
    'MSVCDir','MSVC','MSVCNT','MINGW','watcom'
    ):
    
    try:
        del os.environ[s]
    except:
        pass

import unit_tests
import startup_v1
import startup_v2
import project_test1
import project_test3
import project_test4
import generators_test
import dependency_test
import direct_request_test
import path_features
import relative_sources
import no_type
import chain
import default_build
import main_properties
import use_requirements
import conditionals
import stage
import prebuilt
import project_dependencies
import build_dir
if os.name == 'posix':
    import symlink

