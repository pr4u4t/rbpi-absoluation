set terminal png size 1024,768
#set output "storage.png"
set hidden3d
set isosamples 30,30
set xrange [0:60]
set yrange [0:150]
set xlabel "Producenci [ szt. ]"
set ylabel "Zamówienia [ szt./rocznie ]"
set zlabel "Miejsce na dysku [ MB ]"
set zlabel rotate by 90
set zlabel offset -3
set grid xtics
set grid ytics
set view 62,330
splot 100*x+3934.5*y title "Ilośc miejsca na dysku od ilości zamównień i producentów"
