# Copyright 2007 Stjepan Rajko.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

run_test ()
{
    result=results/$1
    expected=expected/$2
    input=$3
    shift 3
    echo ---------------- | cat >>test.results
    echo $input \| make_template.py $* | cat >>test.results
    echo ---------------- | cat >>test.results
    echo $input | python2.3 ../../../../make_template.py destination=$result "$@"
    diff -b -r $expected $result | cat >>test.results
}

rm test.results
rm -r results

run_test simple_type_in simple_template substituted simple_template 
run_test simple_command_line simple_template "" simple_template text=substituted
run_test simple_ignore simple_template_ignored "" simple_template text=substituted ignore=\$template_text\$
run_test simple_fignore simple_template_fignored "" simple_template text=substituted fignore=.txt
run_test sandbox_command_line sandbox_test "" ../../../../sandbox "author=John Doe,Jane H. Dough" library=test

echo
echo
echo
echo --------------------------------------------------------
echo - Final test.results diff:

cat test.results
