cd /scratch/physdoe_project_root/physdoe_project/nastein/Benchmark_points/pythia_files_highmass

for filename in *.hepmc; do
	sbatch --parsable --export=job_title=${filename} /home/nastein/XiJets/LHEfiles/delphes.sh 
	echo $filename
done

