#!/bin/sh
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=1 ./proj4PT.sh 1 /homes/johnriforgiate/cis520/wiki_dump.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=2 ./proj4PT.sh 2 /homes/johnriforgiate/cis520/wiki_dump.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=4 ./proj4PT.sh 4 /homes/johnriforgiate/cis520/wiki_dump.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=8 ./proj4PT.sh 8 /homes/johnriforgiate/cis520/wiki_dump.txt
sbatch --constraint=elves --time=5:00:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=16 ./proj4PT.sh 16 /homes/johnriforgiate/cis520/wiki_dump.txt
