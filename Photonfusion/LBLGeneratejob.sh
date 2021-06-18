#!/bin/bash

#SBATCH --job-name=PhotonFusion_job250
#SBATCH --mail-user=nastein@umich.edu
#SBATCH --mail-type=FAIL
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=5GB
#SBATCH --time=14:00:00
#SBATCH --account=physdoe_project 
#SBATCH --partition=physdoe
#SBATCH --output=/home/nastein/Photonfusion/%x-%j.log

source /home/nastein/root_build/bin/thisroot.sh
cd /home/nastein/MG5_aMC_v2_6_7/

./bin/mg5_aMC <<EOF
import model loop_qcd_qed_sm
generate a a > a a [noborn=QED]
output /scratch/physdoe_project_root/physdoe_project/nastein/LBL250/${job_title}_${m}
launch /scratch/physdoe_project_root/physdoe_project/nastein/LBL250/${job_title}_${m}
shower=Pythia8
detector=OFF
analysis=OFF
0
set lpp1 3
set lpp2 3
set dsqrt_q2fact1 = 2.0
set dsqrt_q2fact2 = 2.0
set fixed_fac_scale = True
set ebeam 125
set iseed ${m}
set nevents 10000
set pta 2.0
set draa .035
set etaa 2.4
0
EOF

cd /scratch/physdoe_project_root/physdoe_project/nastein/LBL250/${job_title}_${m}/Events/run_01
gunzip tag_1_pythia8_events.hepmc.gz
