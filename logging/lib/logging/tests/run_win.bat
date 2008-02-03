set BOOST_ROOT_DIR="%1"
echo Output is redirected to out.txt
bjam --v2 --boost="%1" --toolset=msvc > out.txt
