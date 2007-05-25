# template script

# Copyright 2007 Stjepan Rajko.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

if template.options.get_boolean('vc8ide', False):

    template.log_message('Processing MSVC 8.0 IDE template.')

    _msvc_build_command = """\t\t\t\tBuildCommandLine="bjam --v2 ../../$(ProjectName) $(ConfigurationName)"
\t\t\t\tReBuildCommandLine="bjam --v2 -a ../../$(ProjectName) $(ConfigurationName)"
\t\t\t\tCleanCommandLine="bjam --v2 --clean ../../$(ProjectName) $(ConfigurationName)"
"""

    _msvc_doc_build_command = """\t\t\t\tBuildCommandLine="cd ../../doc&#x0D;&#x0A;bjam --v2 $(ConfigurationName)"
\t\t\t\tReBuildCommandLine="cd ../../doc&#x0D;&#x0A;bjam --v2 -a $(ConfigurationName)"
\t\t\t\tCleanCommandLine="bjam --v2 --clean ../../$(ProjectName) $(ConfigurationName)"
"""

    template.content_replacement(['.vcproj', '.sln'], '$template_msvc_build_uuid$', str(uuid.uuid4()))
    template.content_replacement(['.vcproj', '.sln'], '$template_msvc_doc_uuid$', str(uuid.uuid4()))
    template.content_replacement(['.vcproj', '.sln'], '$template_msvc_example_uuid$', str(uuid.uuid4()))
    template.content_replacement(['.vcproj', '.sln'], '$template_msvc_test_uuid$', str(uuid.uuid4()))
    template.content_replacement(['.vcproj'], '$template_msvc_build_command$', _msvc_build_command)
    template.content_replacement(['.vcproj'], '$template_msvc_doc_build_command$', _msvc_doc_build_command)

else:

    template.ignore_subdirectory('vc8ide')
