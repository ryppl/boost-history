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


library = Replacement('_library_', raw_input('library name: (e.g., something_new): '))
Library = Replacement('_Library_', library.value.capitalize())
LIBRARY = Replacement('_LIBRARY_', library.value.upper())
boost_library = Replacement('_Boost_Library_', 'Boost.' + Library.value)

_copyright = """_COMMENT_Copyright _year_ _author_.
_COMMENT_Distributed under the Boost Software License, Version 1.0. (See
_COMMENT_accompanying file LICENSE_1_0.txt or copy at
_COMMENT_http://www.boost.org/LICENSE_1_0.txt)
"""

_msvc_build_command = """\t\t\t\tBuildCommandLine="cd ../$(ProjectName)&#x0D;&#x0A;bjam --v2 $(ConfigurationName)"
\t\t\t\tReBuildCommandLine="cd ../$(ProjectName)&#x0D;&#x0A;bjam --v2 -a $(ConfigurationName)"
\t\t\t\tCleanCommandLine="cd ../$(ProjectName)&#x0D;&#x0A;bjam --v2 --clean $(ConfigurationName)"
"""

author_list = raw_input('library author[s]: (e.g., Yours Truly[,Yours D. NotTruly]): ')
author_reversed = list()
for name in author_list.rsplit(','):
    first_space_last = name.rpartition(" ")
    author_reversed.append('[' + first_space_last[2] + ', ' + first_space_last[0] + ']')

authors, comma, last_author = author_list.rpartition(",")
author_list = (authors + " and " + last_author).replace(',', ', ')
file_replacements = FileReplacements(['.hpp', '.cpp', '.v2', '.jam', '.qbk', '.vcproj', '.sln'])
file_replacements.extend([boost_library, Library, LIBRARY, library])
file_replacements.append(Replacement ('_python_copyright_', _copyright.replace('_COMMENT_','# ')))
file_replacements.append(Replacement ('_cpp_copyright_', _copyright.replace('_COMMENT_','// ')))
file_replacements.append(Replacement ('_qbk_copyright_', (_copyright.replace('_COMMENT_','')).replace('http://www.boost.org/LICENSE_1_0.txt','[@http://www.boost.org/LICENSE_1_0.txt]')))
file_replacements.append(Replacement('_author_reversed_', ','.join(author_reversed)))
file_replacements.append(Replacement('_author_', author_list))
file_replacements.append(Replacement('_year_', str(time.localtime().tm_year)))
file_replacements.append(Replacement('_msvc_build_uuid_', str(uuid.uuid4())))
file_replacements.append(Replacement('_msvc_doc_uuid_', str(uuid.uuid4())))
file_replacements.append(Replacement('_msvc_example_uuid_', str(uuid.uuid4())))
file_replacements.append(Replacement('_msvc_test_uuid_', str(uuid.uuid4())))
file_replacements.append(Replacement('_msvc_build_command_', _msvc_build_command))

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

                contents = file_replacements.replace(contents)

                fout = open(os.path.join(new_root, new_name), "w")
                fout.write(contents)
                fout.close()
            else:
                shutil.copy(os.path.join(root, name), os.path.join(new_root, new_name))
