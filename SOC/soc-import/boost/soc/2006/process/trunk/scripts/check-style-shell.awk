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
    emacs_modeline = 0
    vim_modeline = 0
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
}

/vim: syntax=sh/ {
    vim_modeline = 1
}

/mode: shell-script/ {
    emacs_modeline = 1
}

/^[ \t]*#/ {
    next
}

/[$ \t]+_[a-zA-Z0-9]+=/ {
    warn("Variable should not start with an underline")
}

/\$[^0-9'"$?@#*{(|\/]+/ {
    warn("Missing braces around variable name")
}

/=(""|'')/ {
    warn("Assignment to the empty string does not need quotes");
}

/"[a-zA-Z0-9\/.,_-]+"/ {
    warn("Single word does not need to be quoted");
}

/'[a-zA-Z0-9\/.,_-]+'/ {
    warn("Single word does not need to be quoted");
}

/basename[ \t]+/ {
    warn("Use parameter expansion instead of basename");
}

/dirname[ \t]+/ {
    warn("Use parameter expansion instead of dirname");
}

/if[ \t]+(test|![ \t]+test)/ {
    warn("Use [ instead of test");
}

/if.*;[ \t]*fi$/ {
    warn("Avoid using a single-line if conditional");
}

END {
    if (skip)
        warn("Missing NO_CHECK_STYLE_END");
    if (! emacs_modeline || ! vim_modeline)
        warn("Missing mode lines");
    if (error)
        exit 1
}
