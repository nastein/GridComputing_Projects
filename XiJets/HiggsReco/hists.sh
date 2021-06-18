#!/bin/bash

#SBATCH --job-name=XSM_FCC_job
#SBATCH --mail-user=nastein@umich.edu
#SBATCH --mail-type=FAIL
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=5GB
#SBATCH --time=8:00:00
#SBATCH --account=physdoe_project 
#SBATCH --partition=physdoe
#SBATCH --output=/home/nastein/XiJets/%x-%j.log

source /home/nastein/root_build/bin/thisroot.sh

cd /home/nastein/MG5_aMC_v2_6_7/Delphes

root -l examples/Higgs_reco.C'("${job_title}")'

