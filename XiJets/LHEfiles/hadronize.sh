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

cd /home/nastein/XiJets/LHEfiles

LD_LIBRARY_PATH=/home/nastein/MG5_aMC_v2_6_7/HEPTools/lib:$LD_LIBRARY_PATH /home/nastein/MG5_aMC_v2_6_7/HEPTools/MG5aMC_PY8_interface/MG5aMC_PY8_interface ${job_title}

