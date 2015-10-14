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
 
using namespace RooFit; 
using namespace std;
 
int main(int argc, char *argv[]) 
{ 
    doocore::lutils::setStyle();
    // toy==true -> toyMC, toy==false -> tuple
    bool toy = false;
    // Observables 
    RooRealVar mass("obsMass","mass (MeV)", 5230, 5330); 
    //RooRealVar omega("obsOmega","omega", 0, 0.5); 
    string var_eta = "obsEtaSSPion";
    string title_eta = var_eta;
    string const_var_time = "obsTime";
    string const_title_time = const_var_time;
    int bins = 50;
    RooRealVar omega(TString(var_eta), TString(title_eta), 0, 0.5);
    RooRealVar time(TString(const_var_time), TString(const_title_time), 0,15);
    RooRealVar catBkg("catBkg", "Bkg", 1., -1, 100);
    RooRealVar catTagCompOSvsSSPion("catTagCompOSvsSSPion", "catTagComp", -2, 2);    


    // Builds Gaussian signal PDFs 
    RooRealVar sigMean("sigMean","sigMean",5280, 5230, 5330); 
    RooRealVar sigWidth("sigWidth", "sigWidth", 7, 2, 20); 
    RooGaussian PDF_Mass_Sig("PDF_Mass_Sig", "gaussian PDF", mass, sigMean, sigWidth);

    RooRealVar sigWidth2("sigWidth2", "sigWidth2", 15, 2, 20);
    RooGaussian PDF_Mass_Sig2("PDF_Mass_Sig2", "gaussian 2nd PDF", mass, sigMean, sigWidth2);
 
    // Build Gaussian omega Sig PDFs
    RooRealVar par_Omega_Sig_Mean("par_Omega_Sig_Mean","sigMean(omega)",0.35, 0, 0.5); 
    RooRealVar par_Omega_Sig_Width("par_Omega_Sig_Width", "sigWidth(omega)", 0.2, 0.001, 1);
    RooGaussian PDF_Omega_Sig("PDF_Omega_Sig", "gaussian(omega_sg) PDF", omega, par_Omega_Sig_Mean, par_Omega_Sig_Width); 

    // Build Gaussian omega Bkg PDFs
    RooRealVar par_Omega_Bkg_Mean("par_Omega_Bkg_Mean","backgroundMean(omega)",0.25, 0, 0.5); 
    RooRealVar par_Omega_Bkg_Width("par_Omega_Bkg_Width", "backgroundWidth(omega)", 0.05, 0.001, 1);
    RooGaussian PDF_Omega_Bkg("PDF_Omega_Bkg", "gaussian(omega_bkg) PDF", omega, par_Omega_Bkg_Mean, par_Omega_Bkg_Width); 

    // Builds Expo background PDF omegaFrame
    RooRealVar expo("expo", "expo shape parameter", -0.005, -100., 10); 
    RooExponential PDF_Mass_Bkg("PDF_Mass_Bkg", "Exponential PDF", mass, expo); 

    // Builds double mass signal PDF
    RooRealVar nFrac("nFrac", "Fraction of 2 Gaussians", 0.5, 0,1);
    RooAddPdf PDF_Mass_Sig_Comp("PDF_Mass_Sig_Comp", "PDF_Mass_Sig_Comp", RooArgList(PDF_Mass_Sig, PDF_Mass_Sig2), RooArgList(nFrac));

    // Constructs Signal and Background PDFs
    RooProdPdf PDF_Sig("PDF_Sig", "signal PDF", PDF_Mass_Sig_Comp, PDF_Omega_Sig);
    RooProdPdf PDF_Bkg("PDF_Bkg", "bkg PDF", PDF_Mass_Bkg, PDF_Omega_Bkg);


    // Constructs signal+background PDF 
    RooRealVar nSig("nSig","#signal events", 20000,0., 1000000); 
    RooRealVar nBkg("nBkg","#background events", 80000, 0., 1000000); 
    RooAddPdf model("model","model",RooArgList(PDF_Sig,PDF_Bkg), RooArgList(nSig, nBkg));
    RooAddPdf model_mass("model_mass","model_mass",RooArgList(PDF_Mass_Sig_Comp, PDF_Mass_Bkg), RooArgList(nSig, nBkg)); 
   
    doocore::io::EasyTuple *data_mc;
    RooDataSet *data;
    if (toy) {
    // Generates a toyMC sample from composite PDF 
    RooDataSet *data_sig = PDF_Sig.generate(RooArgSet(mass, omega), nSig.getVal()); 
    catBkg.setVal(0.);
    data_sig->addColumns(catBkg);
    RooDataSet *data_bkg = PDF_Bkg.generate(RooArgSet(mass, omega), nBkg.getVal()); 
    catBkg.setVal(1.);
    data_bkg->addColumns(catBkg);
    data = (RooDataSet*) data_sig->Clone();
    data->append(*data_bkg);
    delete data_sig, data_bkg;
    }
    else {
    //data_mc = new doocore::io::EasyTuple("/fhgfs/groups/e5/lhcb/NTuples/Bd2JpsiKS/MC/MC11a_Bd2JpsiKS_Detached_Stripping17_DVv33r4_FTfromDV_v6_20130529_rniet_combined_FitTuple.root", "Bd2JpsiKS",RooArgSet(mass, omega, catBkg));
    data_mc = new doocore::io::EasyTuple("/localdata/NTuples/Bd2JpsiKS/Data/2012/DT2012_Bd2JpsiKS_Detached_Stripping20r0_DVv33r5_FTfromDV_v1_20130701_rniet_combined_bestPV_preselected_20130715_ccauet_FitTuple.root", "Bd2JpsiKS", RooArgSet(mass, omega, time, catTagCompOSvsSSPion));
    data = &data_mc->ConvertToDataSet(Cut("obsMass<=5330 && obsMass>5230 && catTagCompOSvsSSPion!=0"));
    }
   
    // Performs extended Maximum Likelihood (ML) fit of composite PDF to toy data 
    model_mass.fitTo(*data, Extended(),NumCPU(6)); 


    // SPlot
    RooStats::SPlot* sData = new RooStats::SPlot("sData","SPlot",*data, &model_mass, RooArgList(nSig,nBkg));
    RooDataSet dataSig(data->GetName(),data->GetTitle(),data,*data->get(),0,"nSig_sw");
    RooDataSet dataBkg(data->GetName(),data->GetTitle(),data,*data->get(),0,"nBkg_sw");  


    // Creates TTree for Signal
    const TTree* Tree_Sig = dataSig.tree();
    Tree_Sig->SaveAs("signal_tree.root");
    const TTree* Tree_Bkg = dataBkg.tree();
    Tree_Bkg->SaveAs("bkg_tree.root");

    
    // Plots toy data and composite PDF overlaid 
    TCanvas c("c","c",800,600); 
    TLine line(0,0,0.5,0);


    RooPlot* massFrame = mass.frame(); 
    data->plotOn(massFrame);

    model.plotOn(massFrame); 
    model.plotOn(massFrame, Components(PDF_Mass_Bkg),LineStyle(kDashed)); 
    model.plotOn(massFrame, Components(PDF_Mass_Sig),LineStyle(kDotted), LineColor(kRed)); 
    model.plotOn(massFrame, Components(PDF_Mass_Sig2),LineStyle(kDotted), LineColor(kRed)); 


    massFrame->Draw(); 
    doocore::lutils::printPlot(&c,"plot_massFrame","DT2012_Bd2JpsiKS/obsEta/"+TString(var_eta)+"/");
    //doocore::lutils::printPlot(&c,"plot_massFrame","toyMC/");

    // Returns sum of weigths
    double data_sum_entries = data->sumEntries("obsMass<=5330 && obsMass>5230");
    double dataSig_sum_entries = dataSig.sumEntries("obsMass<=5330 && obsMass>5230");
    double dataBkg_sum_entries = dataBkg.sumEntries("obsMass<=5330 && obsMass>5230");

    RooPlot* omegaFrame = omega.frame();
    data->plotOn(omegaFrame, RooFit::Name("OmegaFrame"), MarkerSize(1), MarkerColor(kBlack), LineColor(kBlack), Rescale(data_sum_entries / data_sum_entries), Binning(bins)); 
 
    if (toy){
    model.plotOn(omegaFrame, RooFit::Name("model_plot")); 
    model.plotOn(omegaFrame, RooFit::Name("model_comp_plot"), Components(PDF_Omega_Bkg)); 
    }

    dataSig.plotOn(omegaFrame, RooFit::Name("dataSig_plot"), MarkerStyle(34), MarkerSize(1), MarkerColor(kRed), LineColor(kRed), Rescale(data_sum_entries / dataSig_sum_entries), Binning(bins));
    dataBkg.plotOn(omegaFrame, RooFit::Name("dataBkg_plot"), MarkerStyle(33), MarkerSize(1), MarkerColor(kGreen+2), LineColor(kGreen+2), Rescale(data_sum_entries / dataBkg_sum_entries), Binning(bins));
 
    omegaFrame->Draw();
    line.SetLineColor(kBlue+3.5);
    line.Draw("same");
   

    TLegend *leg = new TLegend(0.14,0.80,0.30,0.95);
    leg->SetFillColor(kWhite);
    leg->SetTextSize(0.04);
    leg->SetTextColor(kBlack);
    leg->AddEntry("OmegaFrame", "data", "lep"); 
    leg->AddEntry("dataSig_plot","dataSig", "lep");
    leg->AddEntry("dataBkg_plot", "dataBkg", "lep");
    leg->Draw();

    doocore::lutils::printPlot(&c, "plot_omegaFrame", "DT2012_Bd2JpsiKS/obsEta/"+TString(var_eta)+"/");
    //doocore::lutils::printPlot(&c, "plot_omegaFrame", "toyMC/");

    delete data, omegaFrame, leg, massFrame, data_mc, sData, Tree_Sig;

} 
