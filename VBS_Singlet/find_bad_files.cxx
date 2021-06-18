#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>

void find_bad_files(std::string current_file)
{
    //std::string scratch = "/scratch/physdoe_project_root/physdoe_project/nastein/leptonic/";
    //std::string current_file = scratch + file;
    std::cout << "Current file = " << current_file << std::endl;
    if (gSystem->AccessPathName((current_file).c_str())) return;
    TFile *fileIn = new TFile((current_file).c_str(),"READ");
    TTree *tree = (TTree*)fileIn->Get("Delphes");
    int num_entries = tree->GetEntries();
    std::cout << "Number of events = " << num_entries << std::endl;
    std::string current_folder;
    current_folder = current_file;
    current_folder.erase(current_folder.end() - 34, current_folder.end());
    std::string delete_command = "rm -rf " + current_folder;
    //std::cout << "here?" << std::endl;
    fileIn->Close();
    //std::cout << "or here?" << std::endl;
    if(num_entries < 2500) gSystem->Exec((delete_command).c_str());
}


int main(int argc, char * argv[]) {
	std::string file = argv[1];
	find_bad_files(file);
	return 0;
}



