#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TAxis.h>
#include <TStyle.h>

using namespace std;

main(){
  doocore::io::EasyTuple et("/fhgfs/groups/e5/lhcb/NTuples/Bd2JpsiKS/MC/MC11a_Bd2JpsiKS_Stripping17_DVv33r4_FTfromDV_v6_20130529_rniet_combined_FitTuple.root", "Bd2JpsiKS");

  std::vector<std::string> variables;
  variables.push_back("catBkg");
  variables.push_back("varMass");
  variables.push_back("varProptime");
  variables.push_back("catTagged");
  variables.push_back("varTag");
  variables.push_back("catTrackType");
  variables.push_back("varOmegaSSPion");


  std::vector<std::string>::const_iterator i;
  for(i=variables.begin(); i !=variables.end(); i++) {

  if (TString(*i) == "varOmegaSSPion") {

	TCanvas canvas("c", "c", 800, 600);

  	int bins = 100; //number of bins
  	int xup, xlow;
 	double limit;
	


	TTree& tree = et.tree();
  	double Mintree = tree.GetMinimum(TString(*i));
  	double Maxtree = tree.GetMaximum(TString(*i));
  	TH1D hist("hist", TString(*i), bins, Mintree, Maxtree);	 
  	tree.Draw(TString(*i)+">>hist","("+TString(*i)+">=0)&&("+TString(*i)+"<0.5)&&(catTrackType==33)&&(catBkg==0)&&(catTagged==1)");


	TH1D hist2("hist2", "hist2", bins, Mintree, Maxtree);  	
	tree.Draw(TString(*i)+">>hist2","("+TString(*i)+">=0)&&("+TString(*i)+"<0.5)&&(catTrackType==55)&&(catBkg==0)&&(catTagged==1)");

	gStyle->SetOptStat(000000000); 
	hist.GetXaxis()->SetTitle(TString(*i));
  	hist.GetYaxis()->SetTitle("Normalized # of Entries");

	hist.DrawNormalized();
  	hist2.SetLineColor(100);
  	hist2.DrawNormalized("same");

	TLegend *leg = new TLegend(0.1,0.75,0.32,0.9);
  	leg->SetHeader(TString(*i));
  	leg->SetTextSize(0.04);
  	leg->AddEntry("hist","longtrack","l");
  	leg->AddEntry("hist2","downstream","l");
  	leg->Draw(); 

	canvas.SaveAs(TString(*i)+".jpg"); 

	}

}
}

