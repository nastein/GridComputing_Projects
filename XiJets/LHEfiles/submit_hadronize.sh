cd /home/nastein/XiJets/LHEfiles

scratch='/scratch/physdoe_project_root/physdoe_project/nastein/Benchmark_points/pythia_files_highmass'

for filename in *.lhe.gz; do
	pythiacmd="pythia_shower_${filename}.cmd"
	echo ${pythiacmd}
	cp pythia_shower.cmd ${pythiacmd}
	output="HEPMCoutput:file         = ${scratch}/${filename}.hepmc"
	filename="Beams:LHEF=${filename}"
	sed -i "s/pythia_shower.cmd/${pythiacmd}/g" ${pythiacmd}
 	sed -i "s/Beams:LHEF=input.lhe.gz/${filename}/g" ${pythiacmd}
	sed -i "s@HEPMCoutput:file         = ouput.hepmc@${output}@g" ${pythiacmd}	
	sbatch --parsable --export=job_title=${pythiacmd} hadronize.sh 
done

