#!/bin/bash

#SBATCH --job-name=ZDecay_job
#SBATCH --mail-user=nastein@umich.edu
#SBATCH --mail-type=FAIL
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=5GB
#SBATCH --time=8:00:00
#SBATCH --account=physdoe_project 
#SBATCH --partition=physdoe
#SBATCH --output=/home/nastein/ZDecay/%x-%j.log

source /home/nastein/root_build/bin/thisroot.sh
cd /home/nastein/MG5_aMC_v2_6_7/

./bin/mg5_aMC <<EOF
set nb_core 1
import model PSeudoSinglet_Hypercharge_Portal_UFO
generate e+ e- > sig a, (sig > a a)
output /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}_${mass}
launch /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}_${mass}
shower=Pythia8
detector=OFF
analysis=OFF
0
set mhi ${mass}
set ebeam 45.59
set nevents 100000
set pta -1.0
set ea 2.0
set draa .035
0
EOF

cd /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}_${mass}/Events/run_01
gunzip tag_1_pythia8_events.hepmc.gz


