dir="/scratch/physdoe_project_root/physdoe_project/nastein"

for mass in 0.1 0.2 0.3 0.4 0.5 0.8 1.0 1.5 2.0 2.5 3.0 3.5 4.0 4.5 5.0 5.5 6.0 7.0 8.0 9.0 10.0 12.0 15.0 20.0 25.0 30.0 40.0 50.0
do
	filename=${dir}/PseudoScalar_ZDecay_${mass}

	sbatch --parsable --export=job_title=${filename},m=${mass} /home/nastein/ZDecay/delphes.sh 
	echo $filename
done

