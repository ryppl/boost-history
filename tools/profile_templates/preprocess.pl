# preprocess.pl
#
# Copyright (c) 2008
# Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

#add "~" to the end of the backup file name
$^I = "~";

#read the whole file at once
undef $/;

$backslashed_lines = qr/(?>(?>\\(?>\n|\r\n)|.)*)/;
$string = qr/(?:\"(?>\\\\|\\\"|[^\"])*\"|'(?>\\\\|\\'|[^'])*')/;
$comment = qr{(?>//$backslashed_lines|/\*(?>[^*]|\*(?!/))*\*/)};  #as far as we are concerned any text between off and on is a comment
$pp = qr/(?:#$backslashed_lines)/;
$ignored = qr/(?>$string|$comment|$pp)/;


while(<>) {
    s/($ignored)|((?:class|struct)[^\{;()#]*\{)/$1?$1:"$2 PROFILE_TRACER\(\); "/ge;
    print "#include <template_profiler.hpp>\n";
    print;
}

