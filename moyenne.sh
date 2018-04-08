if test $# -ne 2
then
    echo $0 fichier_collisions.txt fichier_sortie.txt
    exit 1
fi
sum=0 ; for i in $(cat $1) ; do sum=`expr $sum + $i` ; done ; echo "moyenne : `expr $sum / 1000`" > $2
tmp=0 ; for i in $(cat $1) ; do if test $i -gt $tmp ; then tmp=$i ; fi ; done ; echo "max : $tmp" >> $2
tmp=999999 ; for i in $(cat $1) ; do if test $i -lt $tmp ; then tmp=$i ; fi ; done ; echo "min : $tmp" >> $2
