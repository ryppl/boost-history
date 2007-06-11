#gnuplot file to plot data from results
#which look like that:
#filename vertices edges kolmogorov pushrelabel boost_kolmo hi_pr hi_prw leda leda_static
#========================

set title 'Max-Flow from generated file-problems'
set xlabel '|V|'
set ylabel 'time [s]'
set data style linespoints
set xtics rotate
set xtics (`grep -v filename "results/file.log" | cut -f2 -d' ' | xargs --replace="x" echo  "x,"` 0)
plot 'results/file.log' using 2:4 title "`grep filename results/file.log | cut -f4 -d' '`",\
     'results/file.log' using 2:5 title "`grep filename results/file.log | cut -f5 -d' '`",\
     'results/file.log' using 2:6 title "`grep filename results/file.log | cut -f6 -d' '`",\
     'results/file.log' using 2:7 title "`grep filename results/file.log | cut -f7 -d' '`",\
     'results/file.log' using 2:8 title "`grep filename results/file.log | cut -f8 -d' '`",\
     'results/file.log' using 2:9 title "`grep filename results/file.log | cut -f9 -d' '`",\
     'results/file.log' using 2:10 title "`grep filename results/file.log | cut -f10 -d' '`"
pause -1 "Wait for user input"
set output "file.png"
set terminal png
replot
