SUBDIRS = impl

check_PROGRAMS = \
match_tests \
bug_000002 \
\
scanner_tests \
\
primitives_tests numerics_tests \
\
fundamental_tests parser_traits_tests traverse_tests \
bug_000005 \
\
composite_tests epsilon_tests actions_tests operators_tests directives_tests \
sf_bug_718903 \
\
rule_tests owi_st_tests grammar_tests subrule_tests owi_mt_tests grammar_mt_tests \
\
closure_tests parametric_tests \
bug_000008 \
\
exception_tests \
\
ast_calc_tests \
bug_000001 \
\
if_tests for_tests while_tests lazy_tests \
\
chset_tests loops_tests symbols_tests escape_char_parser_tests scoped_lock_tests\
confix_tests distinct_tests \
sf_bug_719322 bug_000003 bug_000004 bug_000006 bug_000007 \
\
fixed_size_queue_tests file_iterator_tests multi_pass_tests position_iterator_tests\
sf_bug_720917

# Spirit.Core.Kernel
match_tests_SOURCES = match_tests.cpp
bug_000002_SOURCES = bug_000002.cpp

# Spirit.Core.Scanner
scanner_tests_SOURCES = scanner_tests.cpp

# Spirit.Core.Primitive
primitives_tests_SOURCES = primitives_tests.cpp
numerics_tests_SOURCES = numerics_tests.cpp

# Spirit.Core.Meta
fundamental_tests_SOURCES = fundamental_tests.cpp
parser_traits_tests_SOURCES = parser_traits_tests.cpp
traverse_tests_SOURCES = traverse_tests.cpp
bug_000005_SOURCES = bug_000005.cpp

# Spirit.Core.Composite
composite_tests_SOURCES = composite_tests.cpp
epsilon_tests_SOURCES = epsilon_tests.cpp
actions_tests_SOURCES = actions_tests.cpp
operators_tests_SOURCES = operators_tests.cpp
directives_tests_SOURCES = directives_tests.cpp
sf_bug_718903_SOURCES = sf_bug_718903.cpp

# Spirit.Core.Non_Terminal
rule_tests_SOURCES = rule_tests.cpp
owi_st_tests_SOURCES = owi_st_tests.cpp
grammar_tests_SOURCES = grammar_tests.cpp
subrule_tests_SOURCES = subrule_tests.cpp
owi_mt_tests_SOURCES = owi_mt_tests.cpp
grammar_mt_tests_SOURCES = grammar_mt_tests.cpp

# Spirit.Attribute
closure_tests_SOURCES = closure_tests.cpp
parametric_tests_SOURCES = parametric_tests.cpp

# Spirit.Error_Handling
exception_tests_SOURCES = exception_tests.cpp

# Spirit.Tree
ast_calc_tests_SOURCES = ast_calc_tests.cpp
bug_000001_SOURCES = bug_000001.cpp

# Spirit.Dynamic
if_tests_SOURCES = if_tests.cpp
for_tests_SOURCES = for_tests.cpp
while_tests_SOURCES = while_tests.cpp
#if_p_as_parser_tests_SOURCES = if_p_as_parser_tests.cpp
#while_p_as_parser_tests_SOURCES = while_p_as_parser_tests.cpp
#for_p_as_parser_tests_SOURCES = for_p_as_parser_tests.cpp
lazy_tests_SOURCES = lazy_tests.cpp

# Spirit.Utility
chset_tests_SOURCES = chset_tests.cpp
loops_tests_SOURCES = loops_tests.cpp
symbols_tests_SOURCES = symbols_tests.cpp
escape_char_parser_tests_SOURCES = escape_char_parser_tests.cpp
scoped_lock_tests_SOURCES = scoped_lock_tests.cpp
confix_tests_SOURCES = confix_tests.cpp
distinct_tests_SOURCES = distinct_tests.cpp
sf_bug_719322_SOURCES = sf_bug_719322.cpp
bug_000003_SOURCES = bug_000003.cpp
bug_000004_SOURCES = bug_000004.cpp
bug_000006_SOURCES = bug_000006.cpp
bug_000007_SOURCES = bug_000007.cpp
bug_000008_SOURCES = bug_000008.cpp

# Spirit.Iterator
fixed_size_queue_tests_SOURCES = fixed_size_queue_tests.cpp
file_iterator_tests_SOURCES = file_iterator_tests.cpp
multi_pass_tests_SOURCES = multi_pass_tests.cpp
position_iterator_tests_SOURCES = position_iterator_tests.cpp
sf_bug_720917_SOURCES = sf_bug_720917.cpp

EXTRA_DIST = \
runtests.sh \
Jamfile \
for_p_as_parser_tests.cpp \
if_p_as_parser_tests.cpp \
run_regression_tests.py \
run_regression_tests.sh \
sf_bug_742038.cpp \
tree_tests.cpp \
while_p_as_parser_tests.cpp

INCLUDES = -I$(top_srcdir)

TESTS = runtests.sh

