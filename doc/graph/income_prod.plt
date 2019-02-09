set terminal png size 1024,768
#set output "income_prod.png"
set xrange [0:60]
#set yrange [0:50]
set xlabel "Producenci [ szt. ]"
set ylabel "Przychód [ zł ]"
set grid xtics
set grid ytics
plot 3262.5*x title "Prognoza przychodu od ilości producentów" 
