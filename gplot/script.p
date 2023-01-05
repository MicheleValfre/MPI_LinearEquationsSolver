set terminal png size 500,500
set output './gplot/output.png'
set title 'SPEEDUP'
set xlabel 'Processes'
set ylabel 'Speedup'
plot('./gplot/output.dat') w linespoints linestyle 1 pointtype 7 
