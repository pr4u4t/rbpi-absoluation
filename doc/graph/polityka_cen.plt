set terminal png size 1024,768
#set output "polityka_cen.png"
set title "Politytka cenowa"
set key invert reverse Left outside
set key autotitle columnheader
set yrange [0:6500]
set auto x
unset xtics
set xtics nomirror rotate by -45 scale 0
set style data histogram
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.5
#
plot 'polityka_cen.dat' using 2:xtic(1), for [i=3:7] '' using i
#
