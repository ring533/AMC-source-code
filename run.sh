g++ -o main_f main_f.cpp
let counter=1
while read rows
do
qsub <<EOF
#!/bin/sh
# Batch Job
#$ -S /bin/sh
#$ -N "main_f$counter"   
#$ -j y
#$ -o ./main_f.log  
#$ -cwd
#$ -q phy.q          
#$ -p 1         

$rows
EOF
let counter=$counter+1
done < config.sh