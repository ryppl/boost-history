
# Source this files `. <FILE>`.

export BOOST_ROOT="/cygdrive/e/sandbox/boost-trunk.cygwin"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$BOOST_ROOT/stage/lib"
export BOOST_BUILD_PATH="$BOOST_ROOT/tools/build/v2"
export PATH="$PATH:$BOOST_BUILD_PATH/engine/bin.cygwinx86"

echo "HOME=$HOME"
echo "BOOST_ROOT=$BOOST_ROOT"
echo "BOOST_BUILD_PATH=$BOOST_BUILD_PATH"
echo
echo 'Make sure your "\$HOME/user-config.jam" exists and it is correct.'

