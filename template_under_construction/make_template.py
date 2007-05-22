# Copyright 2007 Stjepan Rajko.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import os
import time
import uuid
import shutil

class Replacement(object):
    def __init__(self, template, value):
        self.template = template
        self.value = value
    def replace(self, string):
        return string.replace(self.template, self.value)

class FileReplacements(list):
    def __init__(self, extensions):
        self.process_extensions = extensions
        
    def replace(self, string):
        for item in self:
            string = item.replace(string)
        return string

    def process(self, string):
        for item in self.process_extensions:
            if string.endswith(item):
                return True
        return False


library = Replacement('$template_library$', raw_input('library name: (e.g., something_new): '))
Library = Replacement('$template_Library$', library.value.capitalize())
LIBRARY = Replacement('$template_LIBRARY$', library.value.upper())
boost_library = Replacement('$template_Boost_Library$', 'Boost.' + Library.value)

_copyright = """_COMMENT_Copyright $template_year$ $template_author$.
_COMMENT_Distributed under the Boost Software License, Version 1.0. (See
_COMMENT_accompanying file LICENSE_1_0.txt or copy at
_COMMENT_http://www.boost.org/LICENSE_1_0.txt)
"""

_msvc_build_command = """\t\t\t\tBuildCommandLine="bjam --v2 ../../$(ProjectName) $(ConfigurationName)"
\t\t\t\tReBuildCommandLine="bjam --v2 -a ../../$(ProjectName) $(ConfigurationName)"
\t\t\t\tCleanCommandLine="bjam --v2 --clean ../../$(ProjectName) $(ConfigurationName)"
"""

author_list = raw_input('library author[s]: (e.g., Yours Truly[,Yours D. NotTruly]): ')
author_reversed = list()
for name in author_list.rsplit(','):
    first_space_last = name.rpartition(" ")
    author_reversed.append('[' + first_space_last[2] + ', ' + first_space_last[0] + ']')

authors, comma, last_author = author_list.rpartition(",")
if len(comma) > 0:
    author_list = (authors + " and " + last_author).replace(',', ', ')
else:
    author_list = last_author

file_replacements = FileReplacements(['.hpp', '.cpp', '.v2', '.jam', '.qbk', '.vcproj', '.sln'])
file_replacements.extend([boost_library, Library, LIBRARY, library])
file_replacements.append(Replacement ('$template_python_copyright$', _copyright.replace('_COMMENT_','# ')))
file_replacements.append(Replacement ('$template_cpp_copyright$', _copyright.replace('_COMMENT_','// ')))
file_replacements.append(Replacement ('$template_qbk_copyright$', (_copyright.replace('_COMMENT_','')).replace('http://www.boost.org/LICENSE_1_0.txt','[@http://www.boost.org/LICENSE_1_0.txt]')))
file_replacements.append(Replacement('$template_author_reversed$', ','.join(author_reversed)))
file_replacements.append(Replacement('$template_author$', author_list))
file_replacements.append(Replacement('$template_year$', str(time.localtime().tm_year)))
file_replacements.append(Replacement('$template_msvc_build_uuid$', str(uuid.uuid4())))
file_replacements.append(Replacement('$template_msvc_doc_uuid$', str(uuid.uuid4())))
file_replacements.append(Replacement('$template_msvc_example_uuid$', str(uuid.uuid4())))
file_replacements.append(Replacement('$template_msvc_test_uuid$', str(uuid.uuid4())))
file_replacements.append(Replacement('$template_msvc_build_command$', _msvc_build_command))

if os.path.exists(library.value):
    print 'Directory ' + library.value + ' already exists.'
    do_erase = raw_input('Erase it (please make sure you want to erase the entire directory tree rooted at ' + library.value + ')? [y/n] ')
    if do_erase.lower().startswith('y'):
        print 'Erasing...'
        for root, dirs, files in os.walk(library.value, topdown=False):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                os.rmdir(os.path.join(root, name))
        os.rmdir(library.value)

os.mkdir(library.value)

for root, dirs, files in os.walk(library.template):
    new_root = library.replace(root)
    if os.path.exists(new_root):
        print (new_root)
        for name in dirs:
            if not name.startswith("."):
                new_name = library.replace(name)
                os.mkdir(os.path.join(new_root,new_name))

        for name in files:
            new_name = library.replace(name)
            if (file_replacements.process(name)):
                fin = open(os.path.join(root, name), "r")
                
                contents = fin.read()
                fin.close()

                # only take the content from last $template_start$ on,
                # and convert all line breaks to '\n'
                discarded, sep, contents = contents.rpartition('$template_start$')
                content_lines = contents.splitlines()
                if len(sep) != 0:
                    content_lines.pop(0)
                contents = '\n'.join(content_lines)
                
                contents = file_replacements.replace(contents)

                fout = open(os.path.join(new_root, new_name), "w")
                fout.write(contents)
                fout.close()
            else:
                shutil.copy(os.path.join(root, name), os.path.join(new_root, new_name))
