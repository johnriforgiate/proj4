#!/bin/sh
for i in 1 2 4 8 16
do
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=$i ./proj4PT.sh $i /homes/johnriforgiate/wiki_dump2.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=$i ./proj4PT.sh $i /homes/johnriforgiate/wiki_dump4.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=$i ./proj4PT.sh $i /homes/johnriforgiate/wiki_dump8.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=$i ./proj4PT.sh $i /homes/johnriforgiate/wiki_dump16.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=$i ./proj4PT.sh $i /homes/johnriforgiate/wiki_dump32.txt
done
