set boxwidth 0.5
set style fill solid
set yrange [0:]
set grid
set xlabel "Kraje"
set ylabel "Cena tony mebli [USD]"
set terminal png size 1024,768
#set output "price_weight.png"
plot "price_weight.dat" using 1:3:xtic(2) with boxes title "Cena tony mebli w dolarach amerykańskich" 
