#!/bin/bash

#SBATCH --job-name=PhotonFusion_job
#SBATCH --mail-user=nastein@umich.edu
#SBATCH --mail-type=FAIL
#SBATCH --cpus-per-task=1
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem-per-cpu=5GB
#SBATCH --time=8:00:00
#SBATCH --account=physdoe_project 
#SBATCH --partition=physdoe
#SBATCH --output=/home/nastein/Photonfusion/%x-%j.log

source /home/nastein/root_build/bin/thisroot.sh
cd /home/nastein/MG5_aMC_v2_6_7/

./bin/mg5_aMC <<EOF
set nb_core 1
import model PseudoSinglet_Hypercharge_Portal_UFO
generate a a > a a 
output /scratch/physdoe_project_root/physdoe_project/nastein/Photonfusion250/${job_title}_${m}
launch /scratch/physdoe_project_root/physdoe_project/nastein/Photonfusion250/${job_title}_${m}
shower=Pythia8
detector=OFF
analysis=OFF
0
set mhi ${m}
compute_widths sig --body_decay=3
set lpp1 3
set lpp2 3
set dsqrt_q2fact1 = 2.0
set dsqrt_q2fact2 = 2.0
set fixed_fac_scale = True
set ebeam 250
set nevents 200000
set pta 2.0
set etaa 2.4
set draa .035
0
EOF

cd /scratch/physdoe_project_root/physdoe_project/nastein/Photonfusion250/${job_title}_${m}/Events/run_01
gunzip tag_1_pythia8_events.hepmc.gz


