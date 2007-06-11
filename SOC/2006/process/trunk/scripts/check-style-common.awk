#
# Boost.Process
#
# Copyright (c) 2003, 2004, 2007 Julio M. Merino Vidal.
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt.)
#

function warn(msg) {
    print FILENAME "[" FNR "]: " msg > "/dev/stderr"
    error = 1
}

BEGIN {
    skip = 0
    error = 0
}

/NO_CHECK_STYLE_BEGIN/ {
    skip = 1
    next
}

/NO_CHECK_STYLE_END/ {
    skip = 0
    next
}

/NO_CHECK_STYLE/ {
    next
}

{
    if (skip)
        next

    if (length > 79)
        warn("Line too long to fit on screen")
}

/^ *\t+/ {
    if (! match(FILENAME, "Makefile"))
        warn("Tab character used for indentation");
}

/[ \t]+$/ {
    warn("Trailing spaces or tabs");
}

/[\$]Id:.*$/ {
    if (match($0, "[0-9][0-9][0-9][0-9]/[0-9][0-9]/[0-9][0-9]"))
        year = substr($0, RSTART, 4);
}

/Copyright \(c\)[ \t]+20/ {
    if (! match($0, year))
        warn("Copyright sentence does not include " year);
}

END {
    if (skip)
        warn("Missing NO_CHECK_STYLE_END");
    if (error)
        exit 1
}
