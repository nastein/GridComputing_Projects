#!/bin/bash

#SBATCH --job-name=Zdecay_job
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

cd /home/nastein/MG5_aMC_v2_6_7/Delphes

./DelphesHepMC cards/delphes_card_ILCgen.tcl ${job_title}/Events/run_01/zdecay_${m}.root ${job_title}/Events/run_01/tag_1_pythia8_events.hepmc
