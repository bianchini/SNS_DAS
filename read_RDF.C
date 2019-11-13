#include "ROOT/RDataFrame.hxx"
#include "TCanvas.h"
#include <chrono>

using namespace std::chrono; 

void read_RDF(){

  ROOT::EnableImplicitMT();

  auto start = high_resolution_clock::now();

  ROOT::RDataFrame d("tree", "tree_C.root");

  auto cut = [](double sum, double random, float pz) { return sum>0. && random<pz; };
  auto sum = [](float px, float py) {return (double)(px+py); };

  auto scale1_pz = [](float pz){return (float)(pz*0.1);};
  auto scale2_pz = [](float pz){return (float)(pz*0.2);};
  auto scale3_pz = [](float pz){return (float)(pz*0.3);};
  auto scale4_pz = [](float pz){return (float)(pz*0.4);};
  auto scale5_pz = [](float pz){return (float)(pz*0.5);};

  auto d1 = d.Define("sum", sum, {"px", "py"})
    .Define("scale1_pz", scale1_pz, {"pz"})
    .Define("scale2_pz", scale2_pz, {"pz"})
    .Define("scale3_pz", scale3_pz, {"pz"})
    .Define("scale4_pz", scale4_pz, {"pz"})
    .Define("scale5_pz", scale5_pz, {"pz"});
    
  auto h0 = d1.Filter(cut, {"sum", "random", "pz"}).Histo1D({"h1", "", 100,-3,3}, "px");
  auto h1 = d1.Filter(cut, {"sum", "random", "scale1_pz"}).Histo1D({"h1", "", 100,-3,3}, "px");
  auto h2 = d1.Filter(cut, {"sum", "random", "scale2_pz"}).Histo1D({"h1", "", 100,-3,3}, "px");
  auto h3 = d1.Filter(cut, {"sum", "random", "scale3_pz"}).Histo1D({"h1", "", 100,-3,3}, "px");
  auto h4 = d1.Filter(cut, {"sum", "random", "scale4_pz"}).Histo1D({"h1", "", 100,-3,3}, "px");
  auto h5 = d1.Filter(cut, {"sum", "random", "scale5_pz"}).Histo1D({"h1", "", 100,-3,3}, "px");

  auto stop = high_resolution_clock::now(); 
  auto duration = duration_cast<milliseconds>(stop - start);  
  std::cout << "RDF definitions done in " << duration.count()/1000. << " seconds" << std::endl; 

  std::cout << "Event loop triggered here!" << std::endl;
  start = high_resolution_clock::now(); 
  auto c1 = new TCanvas();
  h0->DrawClone();
  stop = high_resolution_clock::now(); 
  duration = duration_cast<milliseconds>(stop - start);
  std::cout << "h0 done in " << duration.count()/1000. << " seconds" << std::endl; 
  start = high_resolution_clock::now();
  h1->DrawClone("SAME");
  h2->DrawClone("SAME");
  h3->DrawClone("SAME");
  h4->DrawClone("SAME");
  h5->DrawClone("SAME");
  stop = high_resolution_clock::now(); 
  duration = duration_cast<milliseconds>(stop - start);
  std::cout << "All others done in " << duration.count()/1000. << " seconds" << std::endl;

}
