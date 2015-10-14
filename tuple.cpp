#include <iostream> 
 
#include <TCanvas.h>
#include <TFile.h> 
#include <TTree.h> 
//#include <TRandom3.h> 
#include <TH1D.h>

#include "doocore/io/EasyTuple.h" //!!!!
 
using namespace std; 

 
int main(int argc, char *argv[]) 
{ 

    TCanvas canvas("c", "canvas", 800, 600);

    TFile file("test.root", "recreate"); 
    if (!file.IsOpen()) { 
        cerr << "Can't open file." << endl; 
        return 1; 
    } 
 
   
   EasyTuple etuple("tuplefile.root", "Bd2JpsiKsDetachedTuple/DecayTree",
        RooArgSet(B0_MM, B0_P, B0_PT, B0_LOKI_MASS_JpsiKSConstr, B0_TAGOMEGA, B0_TAGOMEGA_OS));
   	TTree& tree = etuple.tree();



 // TRandom3 rand(0); 
 
   // double x;  
   double BO_MM;
 
    //TTree tree("tree", "1-Branch Tree"); 
    //tree.Branch("x", &x); 

 
    //for (int i=0; i<10000; i++) { 
        //generates normally distributed data points
   //     x = rand.Gaus(2, 1); 

 
   //     tree.Fill(); //fills TTree with data
  //  } 
 
  
    
    TH1D *hist = new TH1D("hist", "Histogram", 100, -10, 10);
   // tree.Draw("x>>hist");
    etuple.tree.Draw("BO_MM>>hist");

    canvas.SaveAs("BO_MM.pdf"); 
  

    tree.Write();
    file.Close(); 
 


    return 0; 
} 
