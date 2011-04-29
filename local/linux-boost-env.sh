
# Source this files `. <FILE>`.

export BOOST_ROOT="$HOME/sandbox/boost-trunk.linux"
#export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$BOOST_ROOT/stage/lib"
export BOOST_BUILD_PATH="$BOOST_ROOT/tools/build/v2"
export PATH="$BOOST_BUILD_PATH/engine/bin.linuxx86:$PATH"

echo "HOME=$HOME"
echo "BOOST_ROOT=$BOOST_ROOT"
echo "BOOST_BUILD_PATH=$BOOST_BUILD_PATH"
echo
echo 'Make sure your "\$HOME/user-config.jam" exists and it is correct.'

