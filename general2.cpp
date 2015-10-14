#include "doocore/io/EasyTuple.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <TString.h>
#include <TLine.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <TF1.h>
#include <TMath.h>
#include "background.h"
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <sstream>

using namespace std;

int background (string file_name, string tree_name, string const_var_name, string var_name, double const_var_lower_range, double const_var_mid_range, double const_var_upper_range, double const_var_peak, int bins, double lower_range, double upper_range, bool b_var)
{

doocore::io::EasyTuple et(file_name, tree_name);
// conversion: double -> string
std::stringstream const_var_lower_range_s; const_var_lower_range_s << const_var_lower_range;
std::stringstream const_var_mid_range_s; const_var_mid_range_s << const_var_mid_range;
std::stringstream const_var_upper_range_s; const_var_upper_range_s << const_var_upper_range;
std::stringstream const_var_peak_s; const_var_peak_s << const_var_peak;

// RooFit variables
TTree& tree = et.tree();
RooRealVar mass(TString(const_var_name),"mass (MeV)", const_var_lower_range, const_var_upper_range);
RooRealVar omega("obsOmega","omega", 0, 0.5);
RooRealVar catBkg("catBkg", "Bkg", 1., -1, 100);
RooRealVar nSig_sw("nSig_sw", "nSig_sw", -5, 5);
RooRealVar nBkg_sw("nBkg_sw", "nBkg_sw", -5, 5);
RooDataSet data_sig("data_sig", "data_sig", &tree, RooArgSet (mass, omega, catBkg, nSig_sw), "", "nSig_sw");
RooDataSet data_bkg("data_bkg", "data_bkg", &tree, RooArgSet (mass, omega, catBkg, nBkg_sw), "", "nBkg_sw");

TCanvas canvas("c", "c", 800, 600);
TH1D hist("hist", "hist", bins, const_var_mid_range, const_var_upper_range);
TH1D variable_side (TString(var_name)+"_side", TString(var_name)+"_side", bins, lower_range, upper_range);
TH1D variable_full (TString(var_name)+"_full", TString(var_name)+"_full", bins, lower_range, upper_range);


// Cuts
tree.Draw(TString(const_var_name)+">>hist", "(obsMass>="+TString(const_var_mid_range_s.str())+")&&(obsMass<="+TString(const_var_upper_range_s.str())+")");
tree.Draw(TString(var_name)+">>"+TString(var_name)+"_side", TString("(obsMass>=")+TString(const_var_mid_range_s.str())+")&&(obsMass<="+TString(const_var_upper_range_s.str())+")");


tree.Draw(TString(var_name)+">>"+TString(var_name)+"_full", "(obsMass>="+TString(const_var_lower_range_s.str())+")&&(obsMass<="+TString(const_var_upper_range_s.str())+")");

// Exponential fit function
TF1 fit_function("fit_function", "[0]*(exp([1]*(x-"+TString(const_var_peak_s.str())+")))", const_var_lower_range, const_var_upper_range);
fit_function.SetParameters(100, .005);
hist.Fit("fit_function");

// Errorbars
variable_side.Sumw2();
variable_side.Draw("E1");
variable_full.Draw();

TH1D *variable_sig = (TH1D*) variable_full.Clone(TString(var_name)+"_sig");
double background = (fit_function.Integral(const_var_lower_range,const_var_upper_range))/(fit_function.Integral(const_var_mid_range, const_var_upper_range));
variable_side.Scale(background);
variable_sig->Add(&variable_side, -1);
variable_sig->Draw();

variable_side.SetLineColor(kRed); //red = side part
variable_full.SetLineColor(kBlack); //black = full
variable_sig->SetLineColor(kBlue); //blue = signal

variable_full.Draw();
variable_sig->Draw("same");
variable_side.Draw("same");

TLine line(lower_range,0,upper_range,0);
//Plots of tail with bkg(red) and without bkg(blue)
variable_sig->DrawNormalized();
variable_side.DrawNormalized("same");
line.SetLineColor(kBlue+3.5);
line.Draw("same");

//Legend
TLegend *leg = new TLegend(0.1,0.75,0.3,0.9);
leg->SetTextSize(0.04);
leg->AddEntry(&variable_side,"tail,bkg!=0","l");
leg->AddEntry(variable_sig,"tail, bkg==0","l");
leg->Draw();


canvas.SaveAs(TString(var_name)+"_normalized.pdf");

// RooFit Plots
TH1D bkg0_w("bkg0_w", "bkg=0_w", bins, lower_range,upper_range);
TH1D bkg1_w("bkg1_w", "bkg!=0_w", bins, lower_range,upper_range);
tree.Draw(TString(var_name)+">>bkg0_w", "("+TString(const_var_name)+">="+TString(const_var_lower_range_s.str())+")&&("+TString(const_var_name)+"<="+TString(const_var_upper_range_s.str())+")&&(catBkg==0)");
tree.Draw(TString(var_name)+">>bkg1_w", "("+TString(const_var_name)+">="+TString(const_var_lower_range_s.str())+")&&("+TString(const_var_name)+"<="+TString(const_var_upper_range_s.str())+")&&(catBkg>0)");


RooPlot* omegaFrame = omega.frame();
data_sig.plotOn(omegaFrame, RooFit::MarkerColor(kRed));
data_bkg.plotOn(omegaFrame, RooFit::MarkerColor(kGreen));

RooDataHist BkgSub_Sig("BkgSub_Sig", "BkgSub_Sig", omega, variable_sig);
RooDataHist BkgSub_Bkg("BkgSub_Bkg", "BkgSub_Bkg", omega, &variable_side);
BkgSub_Sig.plotOn(omegaFrame, RooFit::MarkerColor(kBlack));
BkgSub_Bkg.plotOn(omegaFrame, RooFit::MarkerColor(kGreen+3));

omegaFrame->Draw();
line.SetLineColor(kBlack);
line.Draw("same");
canvas.SaveAs("comparisonFrame.pdf");

}
