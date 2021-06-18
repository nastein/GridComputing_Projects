#!/bin/bash

#SBATCH --job-name=SM_FCC_job
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
cd /home/nastein/MG5_aMC_v2_6_7/

./bin/mg5_aMC <<EOF
set nb_core 1
import model Singlet_Extension_UFO
set complex_mass_scheme
define q = u c d s b
define q~ = u~ c~ d~ s~ b~
generate p p > w+ w+ j j QCD=0 WEIGHTED=12, (w+ > l+ vl), (w+ > q q~)
output /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}/${job_title}_${iterator}
launch /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}/${job_title}_${iterator}
shower=Pythia8
detector=OFF
analysis=OFF
0
set ebeam 50000
set alpha 0
set mh__2 5000
compute_widths H --body_decay=2
set iseed ${iterator}
set nevents 2500
0
EOF

cd /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}/${job_title}_${iterator}/Events/run_01
gunzip tag_1_pythia8_events.hepmc.gz

cd /home/nastein/MG5_aMC_v2_6_7/Delphes

./DelphesHepMC cards/FCC/FCChh.tcl /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}/${job_title}_${iterator}/Events/run_01/delphes_events.root /scratch/physdoe_project_root/physdoe_project/nastein/${job_title}/${job_title}_${iterator}/Events/run_01/tag_1_pythia8_events.hepmc
