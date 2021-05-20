#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"
#include "Math/Vector4D.h"
//#include "TVector2.h"

using ROOT::RDataFrame;
using namespace ROOT::VecOps;
//using RNode = ROOT::RDF::RNode;
using rvec_f = const RVec<float>;

void make_plot(){

  TFile* histos = TFile::Open("histos.root", "RECREATE");

  ROOT::EnableImplicitMT();
 
  // Create dataframe from NanoAOD files
  ROOT::RDataFrame df("Events", "SMP-RunIISummer16NanoAODv8-wmassNano_1_Skim.root");

  auto p4 = [](rvec_f pt, rvec_f eta, rvec_f phi, rvec_f m, int idx) {
    return ROOT::Math::PtEtaPhiMVector(pt[idx],eta[idx],phi[idx],m[idx]);
  };
  
  auto abs = [](float w)->float{
    return w>0. ? 1.0 : -1.0;
  };

  auto Q = [](ROOT::Math::PtEtaPhiMVector p1, ROOT::Math::PtEtaPhiMVector p2)->float{
    return (p1+p2).M();
  };

  auto df2 = df
    .Define("weight", abs, {"genWeight"})
    .Define("p4_nu", p4, {"GenPart_pt", "GenPart_eta", "GenPart_phi", "GenPart_mass", "GenPart_preFSRLepIdx1"})
    .Define("p4_mu", p4, {"GenPart_pt", "GenPart_eta", "GenPart_phi", "GenPart_mass", "GenPart_preFSRLepIdx2"})
    .Define("Q",   Q,   {"p4_nu","p4_mu"} );

  auto h_mass = df2.Histo1D<float>({"V_mass", "", 25, 50, 100}, "Q", "weight");   

  histos->cd();
  h_mass->Write();

  histos->Close();

  return;   
}
