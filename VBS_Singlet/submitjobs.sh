master_folder='fcc_5TeV_sina_pt1_leptonic_fixed'
mkdir $scratch/${master_folder}

sim_array_id=$(sbatch --parsable --export=job_title=${master_folder} VBSjobarray.sh)
echo "Submitted simulation array with JobID: ${sim_array_id}"
#sbatch --dependency=afterok:${sim_array_id} --export=job_title=${master_folder} process_job.sh

