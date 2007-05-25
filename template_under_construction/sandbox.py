# template script

template.log_message('Processing Boost Sandbox template.')

template_dir = '$template_library$'

library_name = template.options.get('library', template.options.alnum)
# Copyright 2007 Stjepan Rajko.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

library_name_list = library_name.split("_")
library_name_list_capitalized = list()
for item in library_name_list[:]:
    library_name_list_capitalized.append(item.capitalize())
library_name_capitalized = ' '.join(library_name_list_capitalized)
library_name_concat_capitalized = ''.join(library_name_list_capitalized)

_copyright = """_COMMENT_Copyright $template_year$ $template_author$.
_COMMENT_Distributed under the Boost Software License, Version 1.0. (See
_COMMENT_accompanying file LICENSE_1_0.txt or copy at
_COMMENT_http://www.boost.org/LICENSE_1_0.txt)
"""

author_list = template.options.get('author', template.options.any)
author_reversed = list()
for name in author_list.rsplit(','):
    first_space_last = name.rpartition(" ")
    author_reversed.append('[' + first_space_last[2] + ', ' + first_space_last[0] + ']')

authors, comma, last_author = author_list.rpartition(",")
if len(comma) > 0:
    author_list = (authors + " and " + last_author).replace(',', ', ')
else:
    author_list = last_author

template.name_replacement('$template_library$', library_name)

template.all_content_replacement('$template_library$', library_name)
template.all_content_replacement('$template_Library$', library_name_capitalized)
template.all_content_replacement('$template_LibraryConcat$', library_name_concat_capitalized)
template.all_content_replacement('$template_LIBRARY$', library_name.upper())
template.all_content_replacement('$template_Boost_Library$', 'Boost.' + library_name_capitalized)
template.content_replacement(['.v2', '.jam'], '$template_python_copyright$', _copyright.replace('_COMMENT_','# '))
template.content_replacement(['.hpp', '.cpp'], '$template_cpp_copyright$', _copyright.replace('_COMMENT_','// '))
template.content_replacement(['.qbk'], '$template_qbk_copyright$',(_copyright.replace('_COMMENT_',''))
                             .replace('http://www.boost.org/LICENSE_1_0.txt',
                                      '[@http://www.boost.org/LICENSE_1_0.txt]'))
template.all_content_replacement('$template_author_reversed$', ','.join(author_reversed))
template.all_content_replacement('$template_author$', author_list)
template.all_content_replacement('$template_year$', str(time.localtime().tm_year))

