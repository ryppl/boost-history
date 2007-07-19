
# This script automates the building of graphs for the quickbook documentation.
# In order to run this, you will need GraphViz and ImageMagick installed and
# on path.

# build the movie graph
echo "building co-star graph"
dot -Tpng < movie.dot > movie.png
convert movie.png -resize "80%" movie.png

# build file dependencies
echo "building file dependency graph"
dot -Tpng < files.dot > files.png
dot -Tpng < reverse.dot > reverse.png

echo "building reverse dependency graph"
convert files.png -resize "80%" files.png
convert reverse.png -resize "80%" reverse.png
