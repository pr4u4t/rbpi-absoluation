set terminal png size 1024,768
#set output "visit_prod.png"
set xrange [0:60]
#set yrange [0:50]
set xlabel "Producenci [ szt. ]"
set ylabel "Odwiedziny [ szt./dzien ]"
set grid xtics
set grid ytics
plot 43.3/365*x title "Ilość odwiedzin wymagana do wypełnienia planu sprzedaży od producentów" 
