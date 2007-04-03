#! /bin/sh
#
# Boost.Process
#
# Copyright (c) 2007 Julio M. Merino Vidal.
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt.)
#

#
# A utility to sanity check the coding style of all source files in the
# project tree.
#

progname=${0##*/}

#
# err message
#
err() {
    echo "${progname}: ${@}" 1>&2
    exit 1
}

#
# warn message
#
warn() {
    echo "${progname}: ${@}" 1>&2
}

#
# guess_topdir
#
# Locates the project's top directory and prints its path.  The caller
# must verify if the result is correct or not.
#
guess_topdir() {
    local olddir topdir

    olddir=$(pwd)
    topdir=$(pwd)
    while [ ${topdir} != / ]; do
        if [ -f project-root.jam ]; then
            break
        else
            cd ..
            topdir=$(pwd)
        fi
    done
    cd ${olddir}
    echo ${topdir}
}

#
# find_sources
#
# Locates all source files within the project, relative to the current
# directory, and prints their paths.
#
find_sources() {
    find . \( -name "Jamfile*" -o \
              -name "README*" -o \
              -name "LICENSE*" -o \
              -name "*.awk" -o \
              -name "*.cpp" -o \
              -name "*.hpp" -o \
              -name "*.jam" -o \
              -name "*.qbk" -o \
              -name "*.sh" \
           \) -a \( \! -name "*.svn*" \)
}

#
# guess_formats file
#
# Guesses the formats applicable to the given file and prints the resulting
# list.
#
guess_formats() {
    local formats

    case ${1} in
        *cpp|*hpp)
            formats=common
            ;;
        *sh)
            formats="common shell"
            ;;
        *)
            formats=common
            ;;
    esac

    echo ${formats}
}

#
# check_file file
#
# Checks the validity of the given file.
#
check_file() {
    local format

    for format in $(guess_formats ${1}); do
        awk -f ${topdir}/scripts/check-style-${format}.awk ${1}
    done
}

#
# main [file list]
#
# Entry point.
#
main() {
    topdir=$(guess_topdir)
    if [ ! -f ${topdir}/project-root.jam ]; then
        err "Could not locate the project's top directory"
    fi

    if [ ${#} -gt 0 ]; then
        sources=${@}
    else
        cd ${topdir}
        sources=$(find_sources)
    fi

    for file in ${sources}; do
        file=$(echo ${file} | sed -e "s,\\./,,")

        if [ ! -f ${file} ]; then
            err "Could not open ${file}"
        else
            check_file ${file}
        fi
    done

    true
}

main "${@}"
