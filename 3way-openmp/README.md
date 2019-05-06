### To compile the c code you must use
```
gcc -o proj4OMP proj4OMP.c proj4OMP.h -fopenmp -std=c99
```
### To run the code you need a shell script and the directory
```
sbatch --constraint=elves --time=1:30:00 --mem-per-cpu=4G --nodes=1 --cpus-per-task=16 ./proj4OMP.sh 6 /homes/johnriforgiate/cis520/wiki_dump.txt
```

### To run all commands, run the batching bash script
```
batchOMP.sh
```
