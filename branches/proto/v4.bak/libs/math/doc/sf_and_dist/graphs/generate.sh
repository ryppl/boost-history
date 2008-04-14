#
# Generates PNG files from the SVG masters.
#
# Paths to tools come first, change these to match your system:
#
math2svg='C:\download\open\SVGMath-0.3.1\math2svg.py'
python=/cygdrive/c/Python25/python.exe
inkscape=/cygdrive/c/progra~1/Inkscape/inkscape
# Image DPI:
dpi=96

for svgfile in $*; do
	pngfile=$(basename $svgfile .svg).png
	echo Generating $pngfile
	$inkscape -d $dpi -e $(cygpath -a -w $pngfile) $(cygpath -a -w $svgfile)
done






