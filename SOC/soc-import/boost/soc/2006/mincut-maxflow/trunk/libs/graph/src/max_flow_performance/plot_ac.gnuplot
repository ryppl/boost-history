#gnuplot file to plot data from results
#which look like that:
#filename vertices edges kolmogorov pushrelabel boost_kolmo hi_pr hi_prw
#========================

set title 'Max-Flow from generated ac-problems'
set xlabel '|V|'
set ylabel 'time [ms]'
set data style linespoints
set xtics (`grep -v filename results/ac.log | cut -f2 -d' ' | xargs --replace="x" echo  "x,"` 0)
plot 'results/ac.log' using ($2):4 title "`grep filename results/ac.log | cut -f4 -d' '`",\
     'results/ac.log' using 2:5 title "`grep filename results/ac.log | cut -f5 -d' '`",\
     'results/ac.log' using 2:6 title "`grep filename results/ac.log | cut -f6 -d' '`",\
     'results/ac.log' using 2:7 title "`grep filename results/ac.log | cut -f7 -d' '`",\
     'results/ac.log' using 2:8 title "`grep filename results/ac.log | cut -f8 -d' '`"
pause -1 "Wait for user input"