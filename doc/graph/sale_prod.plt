set terminal png size 1024,768
#set output "sale_prod.png"
set xrange [0:60]
#set yrange [0:50]
set xlabel "Producenci [ szt. ]"
set ylabel "Sztuki sprzedane [ szt. ]"
set grid xtics
set grid ytics
plot 7.95*x title "Prognoza sprzedanych sztuk towaru w zależności od ilości producentów łączne",\
0.75*7.95*x title "Prognoza sprzedanych sztuk towaru w zależności od ilości producentów w Polsce",\
0.25*7.95*x title "Prognoza sprzedanych sztuk towaru w zależności od ilości producentów w Kanadzie"
