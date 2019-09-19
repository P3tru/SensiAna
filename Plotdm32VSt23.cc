//
// Created by zsoldos on 02/12/18.
//

#include <iostream>
#include <TMultiGraph.h>

#include "Plotdm32VSt23.hh"

int main(int argc, char *argv[]) {

  // Create TApp
  TApplication theApp("App", &argc, argv);

  // Parse arguments
  PTParser parserApp(&theApp);
  parserApp.LaunchApp();

  // Set T2K Style for Plots
  PTStyle ptStyle("Custom");
  ptStyle.SetCustomStyleVariables();
  gROOT->SetStyle("Custom");

  auto nbFiles = parserApp.getNFiles();
  auto FInAsimov = parserApp.getFInAsimov();
  std::map<std::string, std::vector<std::string> >::iterator itFInAsimov;
  auto isPlotOnSameCanvas = parserApp.isPlotOnSameCanvas();

  std::map<std::string, PTAsimov> PTAnalysis;
  std::map<std::string, PTAsimov>::iterator itPTAnalysis;

  TCanvas *cContours;
  TH2D *dummy;
  TLegend *leg;   // Make legend
  TColor *ContoursColor;
  std::vector<int> ColorPalette = createColor(ContoursColor);
  std::vector< std::string > SingleAnaName;

  for(itFInAsimov = FInAsimov.begin(); itFInAsimov!=FInAsimov.end(); ++itFInAsimov){

    auto key = itFInAsimov->first;
    SingleAnaName.push_back(key);
    const auto nbFilesPerAnalysis = nbFiles[key];

    PTAsimov ptAsimov[nbFilesPerAnalysis];

    for(int iFile=0; iFile<nbFilesPerAnalysis; iFile++){

      std::string input_trace_filename = MakeMasterName(itFInAsimov->second[iFile]);
      if(!key.empty()) input_trace_filename = key;

      ptAsimov[iFile].setMasterName(input_trace_filename);

      ptAsimov[iFile].setContoursColor(ColorPalette[LITTLE_BOY_BLUE]);
      ptAsimov[iFile].setBestFitColor(ColorPalette[LITTLE_BOY_BLUE]);
      ptAsimov[iFile].setAsimovAColor(ColorPalette[ARCADIA]);

      ptAsimov[iFile].CreateHistograms(itFInAsimov->second[iFile]);

      ptAsimov[iFile].SetBestFitPoint();
      ptAsimov[iFile].PrintBestPoint();

      ptAsimov[iFile].SetAsimovPoint();

      ptAsimov[iFile].CreateContours1Sigma(ptAsimov[iFile].getSA_s23_dm23()->GetMinimum(),
                                           ptAsimov[iFile].getSA_s23_dm23());
      ptAsimov[iFile].CreateContours2Sigma(ptAsimov[iFile].getSA_s23_dm23()->GetMinimum(),
                                           ptAsimov[iFile].getSA_s23_dm23());


      ptAsimov[iFile].CreateCanvasContours();

      ptAsimov[iFile].WriteOutputFile();

    } // END for iFile per Analysis

    PTAnalysis[key] = ptAsimov[0];

  } // END for Analysis

  std::string AnaName = "Comp";
  if(!parserApp.getNameSameCanvas().empty()) AnaName = parserApp.getNameSameCanvas();
  itPTAnalysis = PTAnalysis.begin();

  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  TPad *pad2 = new TPad("pad2","",0,0,1,1);

  if(isPlotOnSameCanvas){

    cContours = new TCanvas(Form("c%s", AnaName.c_str()), Form("c%s", AnaName.c_str()), 800, 600);
    pad1->Draw();
    pad1->cd();

    // Dummy TH2 for plotting
    dummy = new TH2D(Form("%s_%s",
                          AnaName.c_str(),
                          "dummy"),
                     "Asimov Sensitivity",
                     itPTAnalysis->second.getNbBins()[s23],
                     itPTAnalysis->second.getStartValue()[s23],itPTAnalysis->second.getLastValue()[s23],
                     itPTAnalysis->second.getNbBins()[dm32],
                     itPTAnalysis->second.getStartValue()[dm32],itPTAnalysis->second.getLastValue()[dm32]);
    if(!AnaName.empty()) dummy->SetTitle(AnaName.c_str());
    dummy->GetXaxis()->SetTitle("sin^{2}#theta_{23}");
    dummy->GetYaxis()->SetTitle("#Delta m^{2}_{23}");
    dummy->GetYaxis()->SetTitleOffset(1.1);

    dummy->Draw();

    TGraph *gr90Dummy;
    TGraph *gr68Dummy;

    leg = new TLegend(0.65,0.80,0.98,0.98);
    leg->SetTextFont(132);
    leg->SetLineColor(kBlack);

    TMultiGraph *mgBestFit = new TMultiGraph();
    if(!AnaName.empty()) mgBestFit->SetTitle(AnaName.c_str());
    TGraph *grBestFitDummy;

    int iColor=0;
    for(itPTAnalysis = PTAnalysis.begin(); itPTAnalysis!=PTAnalysis.end(); ++itPTAnalysis) {

      itPTAnalysis->second.DrawContours(itPTAnalysis->second.getAsimov_68(),ColorPalette[iColor],2);
      itPTAnalysis->second.DrawContours(itPTAnalysis->second.getAsimov_90(),ColorPalette[iColor],1);

      grBestFitDummy = new TGraph(1);
      grBestFitDummy->SetPoint(0,itPTAnalysis->second.getS23MinVal(),itPTAnalysis->second.getDm23MinVal());
      grBestFitDummy->SetMarkerColor(ColorPalette[iColor]);
      grBestFitDummy->SetMarkerSize(1);
      grBestFitDummy->SetMarkerStyle(kFullCrossX);
      mgBestFit->Add(grBestFitDummy,"p");

      gr90Dummy = new TGraph(1);
      gr68Dummy = new TGraph(1);

      gr90Dummy->SetLineColor(ColorPalette[iColor]);
      gr90Dummy->SetLineWidth(2);
      gr68Dummy->SetLineColor(ColorPalette[iColor]);
      gr68Dummy->SetLineWidth(2);
      gr68Dummy->SetLineStyle(2);

      leg->AddEntry(gr90Dummy,Form("%s 90%% CL",SingleAnaName[iColor].c_str()),"l");
      leg->AddEntry(gr68Dummy,Form("%s 68%% CL",SingleAnaName[iColor].c_str()),"l");

      ++iColor;

    }

    pad2->SetFrameFillStyle(0);
    pad2->SetFillStyle(4000);
    pad2->Draw();
    pad2->cd();

    itPTAnalysis = PTAnalysis.begin();
    mgBestFit->GetYaxis()->SetRangeUser(itPTAnalysis->second.getStartValue()[dm32],
                                        itPTAnalysis->second.getLastValue()[dm32]);
    mgBestFit->Draw("AP");
    mgBestFit->GetXaxis()->SetLimits(itPTAnalysis->second.getStartValue()[s23],
                                     itPTAnalysis->second.getLastValue()[s23]);

    leg->Draw();

  }


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
