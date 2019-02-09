set terminal png size 1024,768
#set output "porownanie.png"

#define sum function
sumcost=0
suminc=0

costs(x)=(sumcost=sumcost+x)
income(x)=(suminc=suminc+x)

#define best fit curve
f(x) = a*x + b
g(x) = c*x + d
p(x) = 1.69*x**4+x

fit f(x) "porownanie.dat" using 1:(costs($3)) via a,b
fit g(x) "porownanie.dat" using 1:(income($4)) via c,d
h(x) = 0.19*g(x) + f(x)

set title "Rozkład przychodu na składowe"
set xlabel "miesiące"
set ylabel "koszty [ zł ]"
set xtics nomirror rotate by -90 scale 0
set grid

set yrange [0:]
set xrange [0:12]

plot 	'+' using 1:(g($1)):(f($1)) with filledcurve title 'Dochód' lt 1, \
		'+' using 1:(f($1)):(h($1)) with filledcurve title 'Podatek dochodowy' lt 2, \
		'+' using 1:(f($1)):0 with filledcurve title 'Koszty stałe i okresowe' lt 3, \
		"porownanie.dat" using 1:0:xtic(2) notitle, \
		p(x) title "Prawdopodobna prognoza przychodów" lt 4 lw 4




#sumcost=0
#suminc=0



#sumval=0
#plot "porownanie.dat" using 1:(add($3)):xtic(2) title "Suma kosztów na koniec miesiąca" with #filledcurves below, f(x) notitle, \
#sumval=0, \
#"porownanie.dat" using 1:(add($4)):xtic(2) title "Suma przychodów na koniec miesiąca", g(x) #notitle
#
#

#dziala cos
#plot "porownanie.dat" using 1:(g($1)):xtic(2) with filledcurve x1
#, h(x) with filledcurve x1, f(x) with filledcurve x1
