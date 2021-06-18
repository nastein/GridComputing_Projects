job_title=Photonfusion250

for mass in {5..170..2};
do
	sim_array_id=$(sbatch --parsable --export=job_title=${job_title},m=${mass} MG_Generatejob.sh)
	echo "Submitted simulation with mass: ${mass}"
done
