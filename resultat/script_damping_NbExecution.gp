set term png size 1900,1000 enhanced font "Terminal,20"

set title "Execution time in nb execution as a function of damping factor"

set key left top
set style line 1 linewidth 3

set xlabel "Damping factor"
set ylabel "Nb Execution"

plot ARG1 using 1:2 t "Time" linestyle 1 with linespoints