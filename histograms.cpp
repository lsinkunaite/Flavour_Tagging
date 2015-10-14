#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TAxis.h>

using namespace std;

main(){
  doocore::io::EasyTuple et("/fhgfs/groups/e5/lhcb/NTuples/B2JpsiX/B2JpsiX_MC11a_Bd2JpsiKS_Stripping17_DVv29r3p1_FTr134334_v1_20120313_fkruse_combined_tupleA.root", "Bd2JpsiKsDetachedTuple/DecayTree");

  doocore::io::EasyTuple te("/fhgfs/groups/e5/lhcb/NTuples/B2JpsiX/B2JpsiX_MC11a_Bd2JpsiKstar_Stripping17_DVv29r3p1_FTr134334_v1_20120512_fkruse_combined_tupleA.root", "BdDetachedTuple/DecayTree");

//vector 'variables'
  std::vector<std::string> variables;
  variables.push_back("B0_MM");
  variables.push_back("B0_P");
  variables.push_back("B0_PT");
  variables.push_back("B0_LOKI_MASS_JpsiConstr");
  variables.push_back("B0_TAGOMEGA");
  variables.push_back("B0_TAGOMEGA_OS");

//iteration
  std::vector<std::string>::const_iterator i;
  for(i=variables.begin(); i !=variables.end(); i++) {
 
  TCanvas canvas("c", "c", 800, 600);
 

  int bins = 400; //number of bins
  int xup, xlow;
  double limit;

 
  TTree& tree = et.tree();
  double Mintree = tree.GetMinimum(TString(*i));
  double Maxtree = tree.GetMaximum(TString(*i));
  TH1D hist("hist", TString(*i), bins, Mintree, Maxtree);
  //tree.Draw(TString(*i)+">>hist", TString(*i)+">0");
  tree.Draw(TString(*i)+">>hist");

  TTree& tree2 = te.tree();
  double Mintree2 = tree2.GetMinimum(TString(*i));
  double Maxtree2 = tree2.GetMaximum(TString(*i));
  TH1D hist2("hist2", "hist2", bins, Mintree2, Maxtree2);
  //tree2.Draw(TString(*i)+">>hist2", TString(*i)+">0");
  tree2.Draw(TString(*i)+">>hist2");


  //counts total
  double total = 0;
  for (int j=0; j != bins; j++) {
    double binContent = hist.GetBinContent(j);
    total += binContent;

    limit = (0.03) * (total);  //percentage(x2) of cut area
  }
   
  //finds lower bin
  double sum = 0;
  for (int k=0; sum<(limit); k++) {  
    double binContent = hist.GetBinContent(k);    
    sum += binContent;    
    xlow = k;
  }  

  //finds upper bin
  sum = 0;
  for (int h=bins; sum<(limit); h--) {
    double binContent = hist.GetBinContent(h);
    sum += binContent; 
    xup = h; 

   }
  
  //lower x & upper x; Mintree as ref. pt.
  double abc = ((Maxtree-Mintree)/((double)bins));
  double xbinlow = Mintree + ((abc)*(xlow));
  double xbinup = Mintree + ((abc)*(xup));


  hist.GetXaxis()->SetTitle(TString(*i));
  hist.GetYaxis()->SetTitle("Normalized # of Entries");
  

  hist.SetAxisRange(xbinlow, xbinup, "X");


  hist.DrawNormalized();
  hist2.SetLineColor(100);
  hist2.DrawNormalized("same");

  TLegend *leg = new TLegend(0.7,0.75,0.9,0.6);
  leg->SetHeader(TString(*i));
  leg->SetTextSize(0.04);
  leg->AddEntry("hist","Bd2JpsiKs","l");
  leg->AddEntry("hist2","Bd","l");
  leg->Draw(); 

  canvas.SaveAs(TString(*i)+".pdf"); 
}
}


