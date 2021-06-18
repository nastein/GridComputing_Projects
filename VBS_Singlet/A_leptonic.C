#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

#include "classes/lep.C"
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TVectorD.h>
#include <TFile.h>

double W_mass = 80.1;

//------------------------------------------------------------------------------

std::vector<double> getPT(TLorentzVector vec) {

  std::vector<double> PT;
  PT.push_back(vec.X());
  PT.push_back(vec.Y());
  return PT;

}

double Dot(std::vector<double> A, std::vector<double> B) {

  return (A[0]*B[0] + A[1]*B[1]);

}

void Analyzer1(std::string input)
{
  gSystem->Load("libDelphes");

  //std::string dir = "../SameSignWW/Events/Runs/";
  std::string inputFile = "/scratch/physdoe_project_root/physdoe_project/nastein/" + input + "/" + input + ".root";
  // Create chain of root trees
  TChain chain("Delphes");chain.Add((inputFile).c_str());

  std::string outfilename = "/scratch/physdoe_project_root/physdoe_project/nastein/" + input + "/" + input + "_hists.root";
  TFile *outfile = new TFile(outfilename.c_str(), "RECREATE");

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  std::cout << "Number of entries = " << numberOfEntries << std::endl;


  // Get pointers to branches used in this analysis
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchFastJet = treeReader->UseBranch("ParticleFlowJet04");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchMET = treeReader->UseBranch("MissingET");


  TH1D *delta_phi_low_hist = new TH1D(("Delta_phi_low_" + input).c_str(), ("Delta_phi_low" + input).c_str(), 10, 0, 3.141);
  TH1D *delta_phi_high_hist = new TH1D(("Delta_phi_high_" + input).c_str(), ("Delta_phi_high_" + input).c_str(), 10, 0, 3.141);

  TH1D *m_transverse_hist = new TH1D(("m_trans_" + input).c_str()< ("m_trans_" + input).c_str(), 15, 0, 700);

  double jet_pair_mass;
  double delta_phi;
  double MissingEnergy;
  Jet *first_jet, *second_jet;
  MissingET *Met;
  double delta_pseudo;
  int num_leptons;
  vector<lep> leptons;
  double lepton_pair_mass;
  double m_transverse;


  TFile *Xsm = new TFile(inputFile.c_str());
  //First element is the cross section, second is the number of events
  TVectorD *Xsm_vec = (TVectorD*)Xsm->Get("File_details");

  double Xsm_num = (*Xsm_vec)[1];
  double Xsm_xsec = (*Xsm_vec)[0];

  //100 TeV Collider
  double weightXsm = Xsm_xsec*1E3*3000/Xsm_num;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {

    delta_phi = 0;
    jet_pair_mass = 0;
    MissingEnergy = 0;
    delta_pseudo = 0;
    num_leptons = 0;
    leptons.clear();
    lepton_pair_mass = 0;
    m_transverse = 0;

    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    // If event contains at least 2 jets
    if(branchJet->GetEntries() > 1)
    {
      first_jet = (Jet*) branchFastJet->At(0);
      second_jet = (Jet*) branchFastJet->At(1);

      if (first_jet->PT > 30 && second_jet->PT > 30 && std::abs(first_jet->Eta ) < 6 && std::abs(second_jet->Eta) < 6) {

        delta_pseudo = std::abs((first_jet->Eta - second_jet->Eta));
        delta_phi = first_jet->Phi - second_jet->Phi;
      }
    }

    if(branchMET->GetEntries() > 0)
    {
      Met = (MissingET *) branchMET->At(0);
      MissingEnergy = Met->MET;
    }

    if(branchElectron->GetEntries() + branchMuon->GetEntries() > 2) continue;

    //Here we're going to grab the leading leptons and compute the invariant mass of the pair
    Electron *elec;
    if(branchElectron->GetEntries() > 0)
    {
      for(int l = 0; l < branchElectron->GetEntries(); l++) {
        elec = (Electron *) branchElectron->At(l);
        lep lepper(elec->PT, elec->P4());
        leptons.push_back(lepper);

      }
    }

    Muon *muon;
    if(branchMuon->GetEntries() > 0)
    {
      for(int m = 0; m < branchMuon->GetEntries(); m++) {
        muon = (Muon *) branchMuon->At(m);
        lep lepper(muon->PT, muon->P4());
        leptons.push_back(lepper);
      }
    }

    if (MissingEnergy < 20) continue;

    jet_pair_mass = (first_jet->P4() + second_jet->P4()).M();
    std::vector<double> lep1PT = getPT(leptons[0]->P4());
    std::vector<double> lep2PT = getPT(leptons[1]->P4());
    std::vector<double> MissingPT =getPT(Met->P4());

    double firstterm = pow(leptons[0]->PT() + leptons[1]->PT() + MissingEnergy,2);
    double secondterm = Dot(lep1PT,lep1PT) + Dot(lep2PT,lep2PT) + Dot(MissingPT, MissingPT) + 2*Dot(lep1PT,lep2PT) + 2*Dot(lep1PT,MissingPT) + 2*Dot(lep2PT,MissingPT);

    m_transverse = firstterm - secondterm;

    if (delta_pseudo > 3.5) {
      if (jet_pair_mass > 500) delta_phi_low_hist->Fill(std::abs(delta_phi), weightXsm);
      if (jet_pair_mass > 1000) delta_phi_high_hist->Fill(std::abs(delta_phi), weightXsm);
      m_transverse_hist->Fill(m_transverse, weightXsm);
    }


  }//End of event loop

  outfile->Write();

}

