job_title=PseudoScalar_ZDecay

for mass in 0.1 0.2 0.3 0.4 0.5 0.8 1.0 1.5 2.0 2.5 3.0 3.5 4.0 4.5 5.0 5.5 6.0 7.0 8.0 9.0 10.0 12.0 15.0 20.0 25.0 30.0 40.0 50.0 
do
	
	sim_array_id=$(sbatch --parsable --export=job_title=${job_title},m=${mass} MG_Generatejob.sh)
	echo "Submitted simulation with mass: ${mass}"
done
