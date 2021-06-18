job_title=LBL250

for mass in {1..40};
do
	sim_array_id=$(sbatch --parsable --export=job_title=${job_title},m=${mass} LBLGeneratejob.sh)
	echo "Submitted LBL simulation"
done
