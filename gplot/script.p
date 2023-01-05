set autoscale
set terminal png size 500,500
set output './gplot/output.png'
set title 'SPEEDUP'
set xlabel 'Processes'
set ylabel 'Speedup'
plot('./gplot/output.dat') using 2:1 smooth bezier with lines
/*w linespoints linestyle 1 pointtype 7 
*/
