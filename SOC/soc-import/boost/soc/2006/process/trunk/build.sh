#!/bin/sh
#
# Boost.Process
# Simple frontend to bjam to ease build tasks.
#
# Copyright (c) 2006 Julio M. Merino Vidal.
#
# Use, modification, and distribution is subject to the Boost Software
# License, Version 1.0.  (See accompanying file LICENSE_1_0.txt or copy
# at http://www.boost.org/LICENSE_1_0.txt.)
#

ProgName=${0##*/}

err() {
    echo "${ProgName}: ${*}" 1>&2
    exit 1
}

test -f project-root.jam -a -d libs/process || \
    err "This script must be run from Boost.Process' top directory"

test -n "${BOOST_BUILD_PATH}" || \
    err "BOOST_BUILD_PATH is unset; cannot locate Boost.Build v2 directory"
test -f ${BOOST_BUILD_PATH}/boost-build.jam || \
    err "BOOST_BUILD_PATH does not point to a Boost.Build v2 directory"

action=${1:-lib}
case ${action} in
    clean)
        rm -rf libs/process/doc/bin libs/process/doc/html
        ;;
    doc)
        cd libs/process/doc
        bjam --v2
        ;;
    lib)
        err "Not yet!"
        ;;
    *)
        err "Unknown action ${action}"
        ;;
esac
