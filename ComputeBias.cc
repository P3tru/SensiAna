//
// Created by zsoldos on 02/12/18.
//

#include <iostream>
#include <TMultiGraph.h>
#include <TLine.h>

#include "ComputeBias.hh"

int main(int argc, char *argv[]) {

  // Create TApp
  TApplication theApp("App", &argc, argv);

  // Parse arguments
  BiasParser parserApp(&theApp);
  parserApp.LaunchApp();

  // Set T2K Style for Plots
  PTStyle ptStyle("Custom");
  ptStyle.SetCustomStyleVariables();
  gROOT->SetStyle("Custom");

  // Set custom color
  TColor *ContoursColor;
  std::vector<int> ColorPalette = createColor(ContoursColor);

  /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// ANALYSIS ///////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  auto cAsimov = parserApp.getCAsimov();
  auto cFDS = parserApp.getCfds();

  PTAsimov asi;
  asi.setMasterName(MakeMasterName(cAsimov));
  asi.CreateHistograms(cAsimov);
  asi.PrintBestPoint();
  asi.getSA_dm23_1D()->SetLineColor(ColorPalette[LITTLE_BOY_BLUE]);

  PTAsimov fds;
  fds.setMasterName(MakeMasterName(cFDS));
  fds.CreateHistograms(cFDS);
  fds.PrintBestPoint();
  fds.getSA_dm23_1D()->SetLineColor(ColorPalette[CHERRY_TOMATO]);

  // Get delta minChi2
  std::cout << "ASIMOV" << std::endl;
  asi.CreateGraphFromTH1(asi.getSA_dm23_1D());
  double min1SigmaAsimov, max1SigmaAsimov;
  double sigmaAsimov = asi.Find1Sigma(&min1SigmaAsimov,&max1SigmaAsimov);

  std::cout << "FDS" << std::endl;
  fds.CreateGraphFromTH1(fds.getSA_dm23_1D());
  double min1SigmaFDS, max1SigmaFDS;
  double sigmaFDS = fds.Find1Sigma(&min1SigmaFDS, &max1SigmaFDS);

  std::cout << std::endl;

  std::cout << "USING Min AvNLL Values" << std::endl;
  double bias = abs(asi.getDm23MinVal() - fds.getDm23MinVal());
  std::cout << "ABS Bias: " << bias << std::endl;
  bias /= sigmaAsimov;
  std::cout << "Bias: " << bias << std::endl;

  std::cout << std::endl;

  std::cout << "USING Mid 1sigma" << std::endl;
  bias = abs(min1SigmaAsimov + (max1SigmaAsimov-min1SigmaAsimov)/2 - (min1SigmaFDS + (max1SigmaFDS-min1SigmaFDS)/2));
  std::cout << "ABS Bias: " << bias << std::endl;
  bias /= sigmaAsimov;
  std::cout << "Bias: " << bias << std::endl;

  /////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// DRAWING ////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  TLegend *leg;   // Make legend
  leg = new TLegend(0.779449, 0.814236, 0.97995, 0.980903);
  leg->SetTextFont(132);
  leg->SetLineColor(kBlack);

  TLine *lSigma = new TLine(asi.getSA_dm23_1D()->GetBinCenter(1),1,
                            asi.getSA_dm23_1D()->GetBinCenter(asi.getSA_dm23_1D()->GetNbinsX()),1);
  lSigma->SetLineWidth(2.);
  lSigma->SetLineStyle(2);
  lSigma->SetLineColor(kBlack);

  TLine *l2Sigma = new TLine(asi.getSA_dm23_1D()->GetBinCenter(1),4,
                            asi.getSA_dm23_1D()->GetBinCenter(asi.getSA_dm23_1D()->GetNbinsX()),4);
  l2Sigma->SetLineWidth(1.);
  l2Sigma->SetLineStyle(2);
  l2Sigma->SetLineColor(kBlack);

  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->SetGrid();
  asi.getSA_dm23_1D()->Draw("L");
  fds.getSA_dm23_1D()->Draw("Lsame");

  asi.getSA_dm23_1D()->GetYaxis()->SetRangeUser(0,10);


  leg->AddEntry(asi.getSA_dm23_1D(),"Asimov","l");
  leg->AddEntry(fds.getSA_dm23_1D(),"FDS","l");

  leg->Draw();
  lSigma->Draw();

  /////////////////////////
  // ...

  std::cout << std::endl;
  std::cout << "Hit Ctrl+C to exit" << std::endl;
  theApp.Run(kTRUE);

  return 0;

}


std::string MakeMasterName(std::string inputname){

  int index = inputname.find_last_of("/\\");
  std::string input_trace_filename = inputname.substr(index+1);
  size_t lastindex = input_trace_filename.find_last_of(".");
  input_trace_filename = input_trace_filename.substr(0, lastindex);
  std::cout << input_trace_filename << std::endl;

  return input_trace_filename;

}
