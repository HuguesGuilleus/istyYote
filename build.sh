#!/bin/bash

# Projet Yoté -- IATIC 3 ISTY
# 2019 GUILLEUS Hugues IATIC3 <hugues.guilleus@ens.uvsq.fr>

OPT_COMPILATION="-Wall -fdiagnostics-color=always `sdl-config --cflags`"
OPT_ASSEMBLAGE="`sdl-config --libs` -lSDL_ttf"

clear
if [[ $# -ge 1 ]]; then
	case $1 in
		"clean" )
			echo "clean"
			rm -rf _obj
			exit 0 ;;
		"rebuild" )
			rm -rf _obj ;;
		*)
			echo "Commande inconnue $1" >&2
			exit 1 ;;
	esac
fi


mkdir -p _obj/

fail=false
for progPath in `find . -name \*.c`
do
	progObjet=`sed 's#.*/\(.*\)\.c#_obj/\1.o#' <<< $progPath`
	progLib=`sed 's#\.c$#.h#' <<< $progPath`

	echo -ne "\e[01;34m=== Compilation [$progPath]"
	if [[
		!( -e $progObjet )
		|| $progObjet -ot $progPath
		|| $progObjet -ot $progLib
	]]
	then
		gcc $OPT_COMPILATION -c $progPath -o $progObjet &>.log
		if [[ $? == 0 ]]; then
			echo -e "\e[32m OK\033[0m"
		else
			echo -e "\e[31m FAIL\033[0m"
			cat .log
			fail=true
		fi
	else
		echo -e "\e[35m CACHE\033[0m"
	fi
done
rm -f .log
$fail && exit 1

which getDoc >/dev/null && echo -e "\e[01;34m=== Documentation\033[0m" && getDoc

echo -e "\e[01;34m=== Assemblage\033[0m"
gcc _obj/*.o -o yote $OPT_ASSEMBLAGE || exit $?

echo -e "\e[01;34m=== Lancement\033[0m"
./yote
