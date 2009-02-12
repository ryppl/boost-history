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
use re 'eval' ;

$backslashed_lines = qr/(?>(?>\\(?>\n|\r\n)|.)*)/;
$string = qr/(?>\"(?>\\\\|\\\"|[^\"])*\"|'(?>\\\\|\\'|[^'])*')/;
$comment = qr{(?>//$backslashed_lines|/\*(?>[^*]|\*(?!/))*\*/)};  #as far as we are concerned any text between off and on is a comment
$pp = qr/(?>#$backslashed_lines)/;
$ignored = qr/(?>$string|$comment|$pp)/;
$parens = qr/(?>\((?>(?>(?>$ignored|[^()])+)|(??{$parens}))*\))/;
$ws = qr/(?:$comment|$pp|\s|(?:\\(?:\n|\r\n)))/;

$class_header = qr/(?>(?>\b(?:class|struct))(?>$ws+\w+)(?>(?>[^(){;=]|$parens|$ignored)*)\{)/;

$control = qr/(?:\b(?:__attribute__|__if_exists|__if_not_exists|for|while|if|catch|switch)\b)/;
$modifiers = qr/(?:\b(?:try|const|volatile)\b)/;
$start = qr/(?:^|\G|[{};])(?>$ws*)/;
$body = qr/(?:(?!$control)(?>$ignored|[^{};]))/;
$end = qr/(?:$parens|\])/;
$body_start = qr/(?>$ws*(?:$modifiers$ws*)*\{)/;

$function_header = qr/(?>($start)($body*?$end)($body_start))/;

while(<>) {
    s/($ignored)|((?>$class_header|$function_header))/$1?$1:"$2 PROFILE_TRACER\(\); "/ge;
    print "#include <template_profiler.hpp>\n";
    print;
}
