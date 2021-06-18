#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

#include <TLorentzVector.h>
#include <TVector3.h>
#include <TVectorD.h>
#include <TFile.h>

double W_mass = 80.1;

//------------------------------------------------------------------------------

double MTWW(TVector3 Wlep_p, TVector3 Whad_p) {
  double Wlep_ET = sqrt(pow(Wlep_p.Perp(),2) + pow(W_mass,2));
  double Whad_ET = sqrt(pow(Whad_p.Perp(),2) + pow(W_mass,2));

  return sqrt(pow(Wlep_ET + Whad_ET,2) - pow(Wlep_p.Px() + Whad_p.Px(),2) - pow(Wlep_p.Py() + Whad_p.Py(),2));

}

double nu_pz(TVector3 lep_p, TVector3 nu_p, double lepton_m) {

  double Elep = sqrt(pow(lep_p.Mag(),2) + pow(lepton_m,2));

  double a = pow(W_mass,2) - pow(lepton_m,2) + 2*(lep_p.Px()*nu_p.Px() + lep_p.Py()*nu_p.Py());
  double C = 4*pow(Elep,2)*pow(nu_p.Perp(),2) - pow(a,2);
  double b = -4*a*lep_p.Pz();
  double A = 4*(pow(Elep,2) - pow(lep_p.Pz(),2));

  double disc = (pow(b,2) - 4*A*C);
  if (disc < 0) disc = 0;

  double sol1 = 1/(2*A)*(-b + sqrt(disc));
  double sol2 = 1/(2*A)*(-b - sqrt(disc));

  //return the smaller of the two solutions for pz
  return (std::abs(sol1) < std::abs(sol2)) ? sol1 : sol2;
}

void A_Semileptonic_FCC_auto(std::string input)
{
  gSystem->Load("libDelphes");

  std::string inputFile = "/scratch/physdoe_project_root/physdoe_project/nastein/" + input + "/" + input + ".root";

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add((inputFile).c_str());

  std::string outfilename = "/scratch/physdoe_project_root/physdoe_project/nastein/" + input + "/" + input + "_hists.root";
  TFile *outfile = new TFile(outfilename.c_str(), "RECREATE");

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();

  std::cout << "Number of entries = " << numberOfEntries << std::endl;
  Long64_t eventCounter = 0;

  // Get pointers to branches used in this analysis
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchMET = treeReader->UseBranch("MissingET");
  TClonesArray *branchFastJet = treeReader->UseBranch("ParticleFlowJet04");
  
// Book histograms
  TH1D *vhadmjjdiff= new TH1D(("VHad_mjj_diff_" + input).c_str(), ("diff" + input).c_str(), 20, -100, 100);
  TH1D *mjj_tag = new TH1D(("tag_mjj_" + input).c_str(), ("M_{ll}" + input).c_str(), 10, 0, 2000);
  TH1D *MWWt_hist_Xsm = new TH1D(("MwwT_Xsm_" + input).c_str(), ("MwwT_Xsm_" + input).c_str(), 10, 0, 1500);
  TH1D *costheta_hist_Xsm = new TH1D(("Costheta_Xsm_" + input).c_str(), ("Costheta_Xsm_" + input).c_str(),10,-1,1);
  TH1D *delta_phi_hist_Xsm = new TH1D(("Delta_phi_Xsm_" + input).c_str(), ("Delta_phi_Xsm_" + input).c_str(), 10, 0, 3.141);

  MissingET *Met;
  double MissingEnergy;

  double lepton_pt;
  double lepton_mass;
  TVector3 lep_p;
  double lepton_eta;
  TVector3 nu_p;
  bool muon_t;
  bool elec_t;
  TLorentzVector lepton_fourV;

  Jet* Vhad_jet1;
  Jet* Vhad_jet2;
  Jet* tag_jet1;
  Jet* tag_jet2;

  double Wmass_diff;
  double largest_mjj;
  double Vhad_idx1, Vhad_idx2;
  double tag_idx1, tag_idx2;
  bool have_good_tag_jets;
  double delta_phi;

  TVector3 Wlep_p;
  TVector3 Whad_p;
  double MTww;
  double delta_pseudo;

  TVector3 COM_momentum;
  TVector3 Wlep_rest_mom;
  TVector3 Wlep_COMp;
  TVector3 Whad_COMp;

  TLorentzVector Wl_fourV;
  TLorentzVector Wh_fourV;
  TLorentzVector COM_fourV;

  double cos_theta_star;

  //Need to open root files to get total number of events and xsec
  TFile *Xsm = new TFile(inputFile.c_str());

  //First element is the cross section, second is the number of events
  TVectorD *Xsm_vec = (TVectorD*)Xsm->Get("File_details");

  double Xsm_num = (*Xsm_vec)[1];
  double Xsm_xsec = (*Xsm_vec)[0];

  //100 TeV Collider
  double weightXsm = Xsm_xsec*1E3*3000/Xsm_num;
  //double weightXsm = 1;

  // Loop over all events
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {

    muon_t = false;
    elec_t = false;
    MissingEnergy = 0;
    lepton_pt = 0;
    Wmass_diff = 10000000;
    largest_mjj = 0;
    MTww = 0;
    delta_pseudo = 0;
    cos_theta_star = 0;
    have_good_tag_jets = false;
    delta_phi = 0;

    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    // Select events with one and only one lepton
    if( (branchMuon->GetEntries() + branchElectron->GetEntries()) != 1 ) {
      continue;
    }

    Muon *muon;
    if(branchMuon->GetEntries() == 1) {
      muon = (Muon *) branchMuon->At(0);
      lepton_pt = muon->PT;
      lepton_eta = muon->Eta;
      lep_p = muon->P4().Vect();
      lepton_fourV = muon->P4();
      muon_t = true;
      lepton_mass = .10566;
    }
    Electron *elec;
    if(branchElectron->GetEntries() == 1) {
      elec = (Electron *) branchElectron->At(0);
      lepton_pt = elec->PT;
      lepton_eta = elec->Eta;
      lep_p = elec->P4().Vect();
      lepton_fourV = elec->P4();
      elec_t = true;
      lepton_mass = .000511;
    }

    //Get the missing energy
    if(branchMET->GetEntries() > 0) {
      Met = (MissingET *) branchMET->At(0);
      MissingEnergy = Met->MET;
      nu_p = Met->P4().Vect();//Don't use the z momentum from here; reconstruct it in a bit
    }

    //Compute the neutrino pz;
    nu_p.SetZ(nu_pz(lep_p, nu_p, lepton_mass));
    //Reconstruct the Leptonic W momentum
    Wlep_p = nu_p + lep_p;

    //Skip events that fail lepton cuts
    if (MissingEnergy < 20 || lepton_pt < 20) continue;

    double diff = 0;
    double mjj = 0;
    Jet *j1;
    Jet *j2;

    // Select event with at least 4 jets (ignore the possibility of Vhad jets merging for now)
    if(branchJet->GetEntries() < 4) continue;
    else
    {
      //Get our Vhad jets
      for (int j = 0; j < branchFastJet->GetEntries(); j++) {
        j1 = (Jet*) branchFastJet->At(j);
        for (int k = j+1; k < branchFastJet->GetEntries(); k++) {
          j2 = (Jet*) branchFastJet->At(k);
          diff = (j1->P4() + j2->P4()).M() - W_mass;
          if (diff < Wmass_diff) {
            Wmass_diff = diff;
            Vhad_idx1 = j; Vhad_idx2 = k;
            Vhad_jet1 = (Jet*) branchFastJet->At(j); Vhad_jet2 = (Jet*) branchFastJet->At(k);
          }
        }
      }

      //Get our tagging jets
      for (int j = 0; j < branchFastJet->GetEntries(); j++) {
        if (j == Vhad_idx1 || j == Vhad_idx2) continue;
        j1 = (Jet*) branchFastJet->At(j);
        for (int k = j+1; k < branchFastJet->GetEntries(); k++) {
          if (k == Vhad_idx1 || k == Vhad_idx2) continue;
          j2 = (Jet*) branchFastJet->At(k);
          mjj = (j1->P4() + j2->P4()).M();
          if (mjj > largest_mjj && j1->PT > 30 && j2->PT > 30 && std::abs(j1->Eta) < 6 && std::abs(j2->Eta) < 6) {
            largest_mjj = mjj;
            tag_idx1 = j; tag_idx2 = k;
            tag_jet1 = (Jet*) branchFastJet->At(j); tag_jet2 = (Jet*) branchFastJet->At(k);
            have_good_tag_jets = true;
          }
        }
      }//end of loop over jets

    }//End if we have 4 jets

    if (!have_good_tag_jets) continue;

    //Compute delta psuedo rapidty for tagging jets (VBS cut)
    delta_pseudo = std::abs((tag_jet1->Eta - tag_jet2->Eta));
    delta_phi = std::abs((tag_jet1->Phi - tag_jet2->Phi));

    //Reconstruct the Hadronic W momentum
    Whad_p = (Vhad_jet1->P4() + Vhad_jet2->P4()).Vect();

    //Okay now we need to reconstruct the center of momentum frame of the Ws
    //First we set their four vectors in the lab frame
    //Then we boost their four momentum to the frame of the combined W+W+ system
    Wl_fourV.SetXYZM(Wlep_p.Px(),Wlep_p.Py(),Wlep_p.Pz(),W_mass);
    Wh_fourV.SetXYZM(Whad_p.Px(),Whad_p.Py(),Whad_p.Pz(),W_mass);
    COM_fourV = Wl_fourV + Wh_fourV;
    COM_momentum = COM_fourV.BoostVector(); //Momentum of WW system
    Wlep_rest_mom = Wl_fourV.BoostVector(); //Momentum of leptonic W boson

    //These are the four vectors of the W bosons in their COM frame (have checked that Sum(P_i) = 0)
    Wl_fourV.Boost(-COM_momentum.Px(),-COM_momentum.Py(),-COM_momentum.Pz());
    Wh_fourV.Boost(-COM_momentum.Px(),-COM_momentum.Py(),-COM_momentum.Pz());

    //Now we go to the frame of the leptonically decaying W and measure the angle between the lepton and the W direction of motion as defined in the COM frame.
    //First boost the charged lepton into the rest frame of the leptonically decaying W
    lepton_fourV.Boost(-Wlep_rest_mom.Px(), -Wlep_rest_mom.Py(), -Wlep_rest_mom.Pz());
    //Now we measure the angle between the charged leptons momentum (in the W rest frame) and the W direction (as defined in the WW COM frame)
    cos_theta_star = cos(lepton_fourV.Vect().Angle(Wl_fourV.Vect()));

    //Fill Histograms
    vhadmjjdiff->Fill(diff);

    //diff is the difference between mass of hadronic w decay jets and true w mass
    if (diff <= 16) {
      mjj_tag->Fill(largest_mjj);
      if (largest_mjj > 600 && delta_pseudo > 3.5) {
        MWWt_hist_Xsm->Fill(MTWW(Wlep_p,Whad_p),weightXsm);
        costheta_hist_Xsm->Fill(cos_theta_star,weightXsm);
        if (largest_mjj > 1100) {
	  delta_phi_hist_Xsm->Fill(delta_phi,weightXsm);	
	}
      }
    }

  }//End of event loop

  outfile->Write();

}//End of code

