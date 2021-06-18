#!/bin/bash

#SBATCH --job-name=process_job
#SBATCH --mail-user=nastein@umich.edu
#SBATCH --mail-type=FAIL
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=5GB
#SBATCH --time=5:00:00
#SBATCH --account=physdoe_project 
#SBATCH --partition=physdoe
#SBATCH --output=/home/nastein/VBS_Singlet/%x-%j.log

source /home/nastein/root_build/bin/thisroot.sh
cd /home/nastein/VBS_Singlet

scratch='/scratch/physdoe_project_root/physdoe_project/nastein/'
cd scratch
for filename in ${job_title}/*/Events/run_01/delphes_events.root; do 
	./find_bad_files $filename; 
done


hadd ${job_title}/${job_title}.root ${job_title}/${job_title}_*/Events/run_01/delphes_events.root
python xsec_reader.py ${job_title}
#cd /home/nastein/MG5_aMC_v2_6_7/Delphes/
#root -l<<EOF
#.L examples/A_Semileptonic_FCC_auto.C
#A_Semileptonic_FCC_auto("${job_title}")
#.q
#EOF
