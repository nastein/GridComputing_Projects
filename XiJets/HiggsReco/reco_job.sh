cd /scratch/physdoe_project_root/physdoe_project/nastein/Benchmark_points/root_files2

for filename in *.root; do
	sbatch --parsable --export=job_title=${filename} /home/nastein/XiJets/HiggsReco/hists.sh 
done

