#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TMath.h>

using namespace std;

main(){
  doocore::io::EasyTuple et("/fhgfs/groups/e5/lhcb/NTuples/Bd2JpsiKS/MC/MC11a_Bd2JpsiKS_Detached_Stripping17_DVv33r4_FTfromDV_v6_20130529_rniet_combined_FitTuple.root", "Bd2JpsiKS");
  TTree& tree = et.tree();

  TCanvas canvas("c", "c", 800, 600);
  TH1D hist("hist", "hist", 100, 5350,5380);
  TH1D omega_side ("omega_side", "omega_side", 100, 0, 0.5);
  TH1D omega_full ("omega_full", "omega_full", 100, 0, 0.5);


  //canvas.SetLogy();
  tree.Draw("obsMass>>hist", "(obsMass>=5350)&&(obsMass<=5380)");
  tree.Draw("obsOmega>>omega_side", "(obsMass>=5350)&&(obsMass<=5380)");
  tree.Draw("obsOmega>>omega_full", "(obsMass>=5180)&&(obsMass<=5380)");

  TF1 fit_function("fit_function", "[0]*(exp([1]*(x-5280)))", 5180,5380);

  hist.Fit("fit_function");
  hist.Draw();

  //canvas.SaveAs("obsMass.pdf"); 
  omega_side.Draw(); 

  //canvas.SetLogy(false);
  canvas.SaveAs("omega_side.pdf");
  omega_full.Draw();
  canvas.SaveAs("omega_full.pdf");

  TH1D *omega_sig = (TH1D*) omega_full.Clone("omega_sig");
  double background = (fit_function.Integral(5180,5380))/(fit_function.Integral(5350, 5380));
  omega_sig->Add(&omega_side, -background);
  omega_sig->Draw();
  canvas.SaveAs("omega_sig.pdf");

  omega_side.SetLineColor(kRed);
  omega_full.SetLineColor(kBlack);
  omega_sig->SetLineColor(kBlue);

  omega_full.Draw();
  omega_sig->Draw("same");
  omega_side.Draw("same");

  canvas.SaveAs("omega.pdf");

  omega_sig->DrawNormalized();
  omega_side.DrawNormalized("same");
  canvas.SaveAs("omega_normalized.pdf");

  TH1D bg0_w("bg0_w", "bg0_w", 100, 0,0.5);
  TH1D bg1_w("bg1_w", "bg1_w", 100, 0,0.5);

  tree.Draw("obsOmega>>bg0_w", "(obsMass>=5180)&&(obsMass<=5380)&&(catBkg==0)");
  tree.Draw("obsOmega>>bg1_w", "(obsMass>=5180)&&(obsMass<=5380)&&(catBkg>0)");
  omega_sig->DrawNormalized();
  bg0_w.SetLineColor(kRed);  
  bg0_w.DrawNormalized("same");
    

  canvas.SaveAs("bg0_w.pdf");
  omega_side.DrawNormalized();
  bg1_w.SetLineColor(kGreen);
  bg1_w.DrawNormalized("same");

 canvas.SaveAs("bg1_w.pdf");

  return 0;
}
