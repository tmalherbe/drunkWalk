#!/usr/bin/bash

# marche aléatoire de 100000 pas
rm *.ps *.txt log.out 2>/dev/null
echo 100000 | ./drunk -s 2>/dev/null > log.out
gnuplot -e "plot \"log.out\" u 1:2 title \"2d random walk with 100000 steps\" w l ; set term postscript; set output \"c.ps\" ; replot"

# graphes des moyennes des coordonnées x, y, de la distance à l'origine en fonction du nombre de pas
# graphe de l'écart-type de la distance à l'origine en fonction du nombre de pas
rm log.out 2>/dev/null
echo 10 > args.txt ; echo 10000 >> args.txt
./drunk -d < ./args.txt 2>/dev/null > log.out
gnuplot -e "plot \"log.out\" u 1:2 title \"mean of x-coordinate as a function of steps number\" w l ; set term postscript; set output \"x-mean.ps\" ; replot"
gnuplot -e "plot \"log.out\" u 1:3 title \"mean of y-coordinate as a function of steps number\" w l ; set term postscript; set output \"y-mean.ps\" ; replot"
gnuplot -e "plot \"log.out\" u 1:4 title \"mean of final distance as a function of steps number\" w l ; set term postscript; set output \"d-mean.ps\" ; replot"
gnuplot -e "plot \"log.out\" u 1:5 title \"square-type of final distance as a function of steps number\" w l ; set term postscript; set output \"d-square.ps\" ; replot"

# graphes des distributions de probabilités des coordonnées x et y
rm log.out 2>/dev/null
echo 10000 > args.txt ; echo 10000 >> args.txt
./drunk -x < ./args.txt 2>/dev/null > log.out
gnuplot -e "set xrange[-500:500]; plot \"log.out\" u 1:2 title \"x-coordinate distribution\" w l ; set term postscript; set output \"x-distrib.ps\" ; replot"
gnuplot -e "set xrange[-500:500]; plot \"log.out\" u 1:3 title \"y-coordinate distribution\" w l ; set term postscript; set output \"y-distrib.ps\" ; replot"

rm *txt
