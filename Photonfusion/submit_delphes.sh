dir="/scratch/physdoe_project_root/physdoe_project/nastein"

for mass in {5..150..2};
#for mass in 173
do
	filename=${dir}/Photonfusion2509/Photonfusion250_${mass}

	sbatch --parsable --export=job_title=${filename},m=${mass} /home/nastein/Photonfusion/delphes.sh 
	echo $filename
done

