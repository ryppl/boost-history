#!/bin/zsh

# define
# HDSTL_ROOT= libs/hdstl

foreach file in $*
  $HDSTL_ROOT/tools/doc.pl $file >! $HDSTL_ROOT/doxy/`basename $file`
end
