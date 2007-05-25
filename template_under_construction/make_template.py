# Copyright 2007 Stjepan Rajko.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import os
import time
import uuid
import shutil
import sys
import getopt
import re

global template

# The Options class holds user-specified options.
class Options(object):
    def __init__(self):
        self.options = dict()
        for arg in sys.argv[1:]:
            option, eq, val = arg.partition('=')
            self.options[option] = val
        self.any = re.compile(r'.*\Z')
        self.boolean = re.compile(r'[yn]\Z')
        self.alnum = re.compile(r'\w+\Z')

    def verify (self, string, regexp):
        if regexp != None and not regexp.match(string):
            raise NameError, string + ' does not match ' + regexp.pattern
        return string

    def make(self, option, value, prompt=None):
        if value==None:
            if prompt==None:
                prompt_text = option + ': '
            else:
                prompt_text = option + ': ' + prompt
            value = raw_input(prompt_text)

        self.options[option] = value
        return value

    def get(self, option, regexp=None, default=None, prompt=None):
        if self.options.has_key(option):
            # option has already been set
            return self.verify(self.options[option], regexp)
        return self.verify(self.make(option, default, prompt), regexp)

    def get_boolean(self, option, default_bool=None, prompt=None):
        if default_bool != None:
            if default_bool:
                default = 'y'
            else:
                default = 'n'
        else:
            default = None
        value = self.get(option, self.boolean, default, prompt)
        return value=='y'

# The Replacement class models a template replacement,
# either for file names or file content.
class Replacement(object):
    def __init__(self, template, value):
        self.template = template
        self.value = value
    def replace(self, string):
        return string.replace(self.template, self.value)

# Replacements is a collection of Replacement objects.
class Replacements(list):
    def replace(self, string):
        for item in self:
            string = item.replace(string)
        return string

# FileSpecificReplacement associates file extensions with a replacement   
class FileSpecificReplacement(Replacement):
    def __init__(self, extensions, key, value):
        Replacement.__init__(self, key, value)
        self.process_extensions = extensions

    def matches(self, name):
        for item in self.process_extensions:
            if name.endswith(item):
                return True
        return False

class FileSpecificReplacements(Replacements):
    def matches(self, name):
        for item in self:
            if item.matches(name):
                return True
        return False

# ContentReplacements contains both templates for specific file extensions
# and all processable file extensions.
class ContentReplacements():
    def __init__(self):
        self.general_replacements = Replacements()
        self.specific_replacements = FileSpecificReplacements()

    def replace(self, name, content):
        # check to see if we should process this file
        if not self.matches(name):
            return content
        for item in self.specific_replacements:
            content = item.replace(content)
        for item in self.general_replacements:
            content = item.replace(content)
        return content
        
    def matches(self, name):
        match = False
        for item in self.specific_replacements:
            if item.matches(name):
                match = True
                break
        return match

# The Template class holds information about a template,
# such as all of its options and replacements.
class Template(object):
    def __init__(self):
        self.template_dir = ''
        self.options = Options()
        self.content_replacements = ContentReplacements()
        self.name_replacements = Replacements()

    def read_content(self, file_name):
        fin = open(file_name, "r")
        content = fin.read()
        fin.close()
        return content

    def submit_content(self, content):
        self.__content__ = content

    def process_content(self, file_name, python=False):
        if python:
            self.__content__ = ''
            execfile(file_name + '.py')
            content = self.__content__
        else:
            if not template.content_replacements.matches(name):
                return None
            content = self.read_content(file_name)

        # only take the content from first $template_start$ on,
        # and convert all line breaks to '\n'
        discarded, sep, content = content.partition('$template_start$')
        if len(sep)==0:
            content = discarded
        
        content_lines = content.splitlines()
        if len(sep) != 0:
            content_lines.pop(0)
        content = '\n'.join(content_lines)
                
        return self.replace_content(file_name, content)

    def log_message(self, string):
        print '/--------------------'
        print '| ' + string
        print '/'

    def name_replacement(self, key, value):
        self.name_replacements.append(Replacement(key, value))

    def all_content_replacement(self, key, value):                    
        self.content_replacements.general_replacements.append(Replacement(key, value))

    def content_replacement(self, extensions, key, value):                    
        self.content_replacements.specific_replacements.append(
            FileSpecificReplacement(extensions, key, value))

    def replace_name(self, name):
        return self.name_replacements.replace(name)

    def replace_content(self, name, content):
        return self.content_replacements.replace(name, content)

    def examine(self, directory):
        file_list = list()
        directory_list = list()
        python_list = list()

        for root, dirs, files in os.walk(directory):
            print root
            self.__dirs_clear__ = list()
            self.__files_clear__ = list()

            for name in files:
                pathname = (os.path.join(root, name))
                if name.endswith('.py'):
                    content = self.read_content(os.path.join(root, name))

                    if content.startswith('# template script'):
                         execfile(pathname)
                    elif content.startswith('# template module'):
                        module_name,py,nothing = name.rpartition('.py')
                        sys.path.insert(os.path.abspath(root), 0)
                        mod = __import__(module_name, globals(),  locals(), [], -1)
                        sys.path.remove(os.path.abspath(root))
                        mod.do_smt()
                    elif content.startswith('# template file'):
                        python_list.append(pathname[0:len(pathname)-3])
                    else:
                        file_list.append(pathname)
                else:
                    file_list.append(pathname)

            for name in self.__files_clear__:
                file_list.remove(os.path.join(root, name))

            for name in dirs:
                if name.startswith('.'):
                    self.ignore_subdirectory(name)
            for name in self.__dirs_clear__:
                dirs.remove(name)
            for name in dirs:
                directory_list.append( (os.path.join(root, name)) )
            

        return directory_list, file_list, python_list

    def ignore_subdirectory(self, name):
        self.__dirs_clear__.append(name)

    def ignore_subfile(self, name):
        self.__files_clear__.append(name)
        
def set_content(name, content):
    fout = open(name, "w")
    fout.write(content)
    fout.close()

# The main processing script.

# template is a global variable accessible by template scripts.
template = Template()

# We first read a template script file and run it.
template_file = template.options.get("template", template.options.any)
template.log_message('Executing template file ' + template_file + '...')
execfile(template_file)

# We then traverse the template tree and prepare a list of
# directories and files to be created, and process any found template
# scripts.
template.log_message('Examining the template project tree...')

directory_list, file_list, python_list = template.examine(template_dir)

project_dir = template.replace_name(template_dir)

if os.path.exists(project_dir):
    print 'Directory ' + project_dir + ' already exists.'
    do_erase = template.options.get_boolean(
        'erase', None, 'Erase it (please make sure you want to erase the entire directory tree rooted at ' + project_dir + ')? [y/n] ')
    if do_erase:
        template.log_message('Erasing...')
        for root, dirs, files in os.walk(project_dir, topdown=False):
            for name in files:
                os.remove(os.path.join(root, name))
            for name in dirs:
                os.rmdir(os.path.join(root, name))
        os.rmdir(project_dir)

template.log_message('Creating file tree from template...')

os.mkdir(project_dir)

for name in directory_list:
    new_name = template.replace_name(name)
    print new_name
    os.mkdir(new_name)

for name in python_list:
    new_name = template.replace_name(name)
    content = template.process_content(name, True)
    set_content(new_name, content)

for name in file_list:
    new_name = template.replace_name(name)
    content = template.process_content(name)
    if content:
        set_content(new_name, content)
    else:
        shutil.copy(name, new_name)
