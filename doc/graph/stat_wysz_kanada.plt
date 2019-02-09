set boxwidth 0.5
set style fill solid
set yrange [0:]
set grid
set xlabel "miesiące"
set ylabel "wyszukiwania [ tysiące ]"
set terminal png size 1024,768
#set output "stat_wysz_kanada.png"
plot "stat_wysz_kanada.dat" using 1:3:xtic(2) with boxes title "Statystyki wyszukiwania frazy 'furniture store' w Kanadzie"
