/********************************
*                               *
* Adds s-weights to data sample *
*                               *
********************************/


#include <TCanvas.h> 
#include <RooRealVar.h> 
#include <RooDataSet.h> 
#include <RooStats/SPlot.h>
#include <RooAbsPdf.h> 
#include <RooAddPdf.h> 
#include <RooGaussian.h>
#include <RooExponential.h> 
#include <RooProdPdf.h>
#include <RooConstVar.h> 
#include <RooPlot.h> 
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <RooAbsDataStore.h>
#include <RooTreeDataStore.h>
#include <TLine.h>
#include "doocore/io/EasyTuple.h"
#include <string>
#include "doocore/lutils/lutils.h"
#include <TLegend.h>
#include <TString.h>
#include <TFile.h>
#include <RooCategory.h>
 
using namespace RooFit; 
using namespace std;

int main(int argc, char *argv[]) 
{ 
    doocore::lutils::setStyle();
    
    string file_name = "/localdata/NTuples/Bd2JpsiKS/Data/2012/DT2012_Bd2JpsiKS_Detached_Stripping20r0_DVv33r5_FTfromDV_v1_20130701_rniet_combined_bestPV_preselected_20130715_ccauet_FitTuple.root";
    string tree_name = "Bd2JpsiKS";

    // Initial parameters 
    double mass_lower_bound = 5230;
    double mass_upper_bound = 5330;
    double mass_peak = 5280;
    double omega_lower_bound = 0;
    double omega_upper_bound = 0.5;
    double obsTime_lower_bound = 0;
    double obsTime_upper_bound = 30;
    double sigWidth_lower_bound = 2;
    double sigWidth_upper_bound = 20;
    double sigWidth_peak = 7;
    double sigWidth2_peak = 15;
    double par_Omega_Sig_Mean_peak = 0.35;
    double par_Omega_Sig_Width_peak = 0.2;
    double par_Omega_Sig_Width_lower_bound = 0.001;
    double par_Omega_Sig_Width_upper_bound = 1;
    double par_Omega_Bkg_Mean_peak = 0.25;
    double par_Omega_Bkg_Width_peak = 0.05;
    double expo_lower_bound = -100.;
    double expo_upper_bound = 10;
    double expo_peak = -0.005;
    double gaussian_fraction = 0.5;
    double n_events = 1000000;
    double nSig_events = 20000;
    double nBkg_events = 80000;


    std::vector<string>var_eta;
    var_eta.push_back("obsEtaAll");
    var_eta.push_back("obsEtaOS");
    var_eta.push_back("obsEtaSSPion");


    std::vector<string>::iterator i;
    for(i=var_eta.begin(); i !=var_eta.end(); i++) {

    string title_eta = *i;

    // Observables
    RooRealVar mass("obsMass","mass (MeV)", mass_lower_bound, mass_upper_bound); 
    RooRealVar omega(TString(*i), TString(title_eta), omega_lower_bound, omega_upper_bound);
    RooRealVar obsTime("obsTime", "obsTime", obsTime_lower_bound, obsTime_upper_bound);
    RooRealVar catVar_obsTime("catVar_obsTime", "catVar",1);

    // Builds Gaussian signal PDFs 
    RooRealVar sigMean("sigMean","sigMean",mass_peak, mass_lower_bound, mass_upper_bound); 
    RooRealVar sigWidth("sigWidth", "sigWidth", sigWidth_peak, sigWidth_lower_bound, sigWidth_upper_bound); 
    RooGaussian PDF_Mass_Sig("PDF_Mass_Sig", "gaussian PDF", mass, sigMean, sigWidth);
    RooRealVar sigWidth2("sigWidth2", "sigWidth2", sigWidth2_peak, sigWidth_lower_bound, sigWidth_upper_bound);
    RooGaussian PDF_Mass_Sig2("PDF_Mass_Sig2", "gaussian 2nd PDF", mass, sigMean, sigWidth2);
 
    // Builds Gaussian omega Sig PDFs
    RooRealVar par_Omega_Sig_Mean("par_Omega_Sig_Mean","sigMean(omega)",par_Omega_Sig_Mean_peak, omega_lower_bound, omega_upper_bound); 
    RooRealVar par_Omega_Sig_Width("par_Omega_Sig_Width", "sigWidth(omega)", par_Omega_Sig_Width_peak, par_Omega_Sig_Width_lower_bound, par_Omega_Sig_Width_upper_bound);
    RooGaussian PDF_Omega_Sig("PDF_Omega_Sig", "gaussian(omega_sg) PDF", omega, par_Omega_Sig_Mean, par_Omega_Sig_Width); 

    // Builds Gaussian omega Bkg PDFs
    RooRealVar par_Omega_Bkg_Mean("par_Omega_Bkg_Mean","backgroundMean(omega)",par_Omega_Bkg_Mean_peak, omega_lower_bound, omega_upper_bound); 
    RooRealVar par_Omega_Bkg_Width("par_Omega_Bkg_Width", "backgroundWidth(omega)", par_Omega_Bkg_Width_peak, par_Omega_Sig_Width_lower_bound, par_Omega_Sig_Width_upper_bound);
    RooGaussian PDF_Omega_Bkg("PDF_Omega_Bkg", "gaussian(omega_bkg) PDF", omega, par_Omega_Bkg_Mean, par_Omega_Bkg_Width); 

    // Builds Expo background PDF 
    RooRealVar expo("expo", "expo shape parameter", expo_peak, expo_lower_bound, expo_upper_bound); 
    RooExponential PDF_Mass_Bkg("PDF_Mass_Bkg", "Exponential PDF", mass, expo); 

    // Builds double Gaussian mass signal PDF
    RooRealVar nFrac("nFrac", "Fraction of 2 Gaussians", gaussian_fraction, 0,1);
    RooAddPdf PDF_Mass_Sig_Comp("PDF_Mass_Sig_Comp", "PDF_Mass_Sig_Comp", RooArgList(PDF_Mass_Sig, PDF_Mass_Sig2), RooArgList(nFrac));

    // Constructs Signal and Background PDFs
    RooProdPdf PDF_Sig("PDF_Sig", "signal PDF", PDF_Mass_Sig_Comp, PDF_Omega_Sig);
    RooProdPdf PDF_Bkg("PDF_Bkg", "bkg PDF", PDF_Mass_Bkg, PDF_Omega_Bkg);

    // Constructs signal+background PDF 
    RooRealVar nSig("nSig","#signal events", nSig_events,0., n_events); 
    RooRealVar nBkg("nBkg","#background events", nBkg_events, 0., n_events); 
    RooAddPdf model("model","model",RooArgList(PDF_Sig,PDF_Bkg), RooArgList(nSig, nBkg));
    RooAddPdf model_mass("model_mass","model_mass",RooArgList(PDF_Mass_Sig_Comp, PDF_Mass_Bkg), RooArgList(nSig, nBkg));

    doocore::io::EasyTuple *data_mc;
    RooDataSet *data;
    data_mc = new doocore::io::EasyTuple(file_name, tree_name, RooArgSet(mass, omega, obsTime, catVar_obsTime));
    data = &data_mc->ConvertToDataSet();
    // Performs extended Maximum Likelihood (ML) fit of composite PDF
    model_mass.fitTo(*data, Extended(),NumCPU(6)); 

    // SPlot
    RooStats::SPlot* sData = new RooStats::SPlot("sData","SPlot",*data, &model_mass, RooArgList(nSig,nBkg));
    RooDataSet dataSig(data->GetName(),data->GetTitle(),data,*data->get(),0,"nSig_sw");
    
    const TTree* Tree_Sig = dataSig.tree();
    Tree_Sig->SaveAs(TString(*i)+"_signal_sw.root");

    delete data, data_mc;
}
} 

