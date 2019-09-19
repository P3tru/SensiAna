//
// Created by zsoldos on 02/12/18.
//

#include <iostream>

#include <TMath.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TGaxis.h>

#include "PTAsimov.h"

#define AvNLL_INFINITY 999999.
#define AvNLL_BIN_INIT -1

/////////////////////////////////////////////////////////////////////
/// Utils functions outside PTAsimov Class //////////////////////////
/////////////////////////////////////////////////////////////////////

void PrepareBestPoint(TGraph* gr, int Color = kRed){

  gr->SetMarkerSize(1);
  gr->SetMarkerStyle(kFullCrossX);
  gr->SetMarkerColor(Color);

}

/////////////////////////////////////////////////////////////////////
/// BEGINING PTAsimov drawing class /////////////////////////////////
/////////////////////////////////////////////////////////////////////

PTAsimov::PTAsimov() {

  InitializeDefaultParams();

  Sensia = new TChain("SensiTree");
  Sensih = new TChain("SensiHeader");

  globalMinLL = AvNLL_INFINITY;
  globalMinBin = AvNLL_BIN_INIT;

  dm23MinLL = AvNLL_INFINITY;
  dm23MinBin = AvNLL_BIN_INIT;
  dm23MinVal = 0.;

  s23MinLL = AvNLL_INFINITY;
  s23MinBin = AvNLL_BIN_INIT;
  s23MinVal = 0.;

  ContoursColor = kBlack;
  BestFitColor = kRed;
  AsimovAColor = kBlue;

  masterName.clear();

}

PTAsimov::~PTAsimov() {

  delete Sensia;
  delete Sensih;

  delete SA_s23_dm23;
  delete SA_s23_1D; delete SA_dm23_1D;

  delete best_all;

}

void PTAsimov::InitializeDefaultParams(){

  nbBins[s13]      = 81;
  nbBins[delta]    = 51;
  nbBins[dm32]     = 28; nbBins[s23]     = 81;
  nbBins[dm32_bar] = 28; nbBins[s23_bar] = 81;

  refValue[s13]      = 0.0830;
  refValue[delta]    = -1.601;
  refValue[dm32]     = 2.509e-3; refValue[s23]     = 0.528;
  refValue[dm32_bar] = 2.509e-3; refValue[s23_bar] = 0.528;

  StartValue[s13]      = 5.95e-2;
  StartValue[delta]    = -PI;
  StartValue[dm32]     = 2.2e-3; StartValue[s23]     = 0.3;
  StartValue[dm32_bar] = 2.2e-3; StartValue[s23_bar] = 0.3;

  LastValue[s13]      = 0.1405;
  LastValue[delta]    = PI;
  LastValue[dm32]     = 3.0e-3; LastValue[s23]     = 0.7;
  LastValue[dm32_bar] = 3.0e-3; LastValue[s23_bar] = 0.7;

}

void PTAsimov::InitializeParamsFromSensiHeader() {

  Sensih->GetEntry(0);

  for(int i =0; i< N_OSC_PAR; i++) {

    Sensih->SetBranchAddress(Form("NbBins_%s",OscParNames[i]),&nbBins[i]);
    Sensih->SetBranchAddress(Form("refValue%s",OscParNames[i]),&refValue[i]);
    Sensih->SetBranchAddress(Form("StartValue%s",OscParNames[i]),&StartValue[i]);
    Sensih->SetBranchAddress(Form("LastValue%s",OscParNames[i]),&LastValue[i]);

  }

}

void PTAsimov::SetNumuHistograms() {

  SA_s23_dm23 = new TH2D(Form("%s_%s",
                              masterName.c_str(),
                              "SA_s23_dm23"),
                         "Asimov Sensitivity",
                         // nbBins[s23], StartValue[s23], LastValue[s23],
                         nbBins[dm32], StartValue[dm32], LastValue[dm32],
                         nbBins[dm32], StartValue[dm32], LastValue[dm32]);

  SA_dm23_1D = new TH1D(Form("%s_%s",
                             masterName.c_str(),
                             "SA_dm23_1D"),
                        "Asimov Sensitivity",
                        nbBins[dm32], StartValue[dm32], LastValue[dm32]);

  SA_s23_1D =  new TH1D(Form("%s_%s",
                             masterName.c_str(),
                             "SA_s23_1D"),
                        "Asimov Sensitivity",
                        nbBins[dm32], StartValue[dm32], LastValue[dm32]);
                        // nbBins[s23], StartValue[s23], LastValue[s23]);

}

void PTAsimov::SetSensiTree(std::string inputName, Int_t *Indexa, double *AvNLLtota) {

  Sensia->Add(inputName.c_str());

  if(Sensia->GetEntries() == 0){
    std::cerr << "Can't open TChain SensiTree inside " << inputName << std::endl;
    std::cerr << "Check if file exist... " << std::endl;
    exit(EXIT_FAILURE);
  }

  Sensia->SetBranchAddress("AvNLLtot", AvNLLtota);
  Sensia->SetBranchAddress("Index", Indexa);

  Sensih->Add(inputName.c_str());

  if(Sensih->GetEntries() > 0){

    InitializeParamsFromSensiHeader();

  } else {

    std::cerr << "Can't find header info in SensiHeader" << std::endl;
    std::cerr << "Will load default parameter from PTAsimov class" << std::endl;

  }

}

void PTAsimov::FindAvNLLMin(Int_t *Indexa, double *AvNLLtota) {

  for(int i=0; i<Sensia->GetEntries(); i++) {
    Sensia->GetEntry(i);
    if (*AvNLLtota < globalMinLL && *AvNLLtota > 0) {
      globalMinLL = *AvNLLtota;
      globalMinBin = *Indexa;
    }
  }

}

void PTAsimov::CreateNumuContours(Int_t *Indexa, double *AvNLLtota) {

  for(int i=0; i<Sensia->GetEntries(); i++) {

    Sensia->GetEntry(i);

    if (*AvNLLtota < 0) {
      std::cout << " Asimov Negative likelihood : " << *AvNLLtota << " " << globalMinLL << std::endl;
      *AvNLLtota = AvNLL_INFINITY;
    }


    SA_s23_dm23->SetBinContent((*Indexa % nbBins[dm32]) + 1,
                               TMath::FloorNint(*Indexa / nbBins[dm32]) + 1,
                               2 * (*AvNLLtota - globalMinLL));
    // exp(-(*AvNLLtota - globalMinLL)));

  }

  std::cout << "Asimov minimum : " << globalMinLL << std::endl;

}

void PTAsimov::Create1DYSensi(TH2D *hContours, TH1D *h1D, double *AvNLL, double *MinVal) {

  double min_a_temp = AvNLL_INFINITY;

  for(int i=0; i<hContours->GetNbinsY(); i++){

    double temp = AvNLL_INFINITY;

    for(int j=0; j<hContours->GetNbinsX(); j++){
      if(hContours->GetBinContent(j+1,i+1) < temp) temp = hContours->GetBinContent(j+1,i+1);
    }

    h1D->SetBinContent(i+1,temp);

    if(min_a_temp > temp){
      min_a_temp = temp;
      *AvNLL = h1D->GetBinContent(i+1);
      *MinVal = h1D->GetBinCenter(i+1);
    }

  }

}

void PTAsimov::Create1DXSensi(TH2D *hContours, TH1D *h1D, double *AvNLL, double *MinVal) {

  double min_a_temp = AvNLL_INFINITY;

  for(int i=0; i<hContours->GetNbinsX(); i++){

    double temp = AvNLL_INFINITY;

    for(int j=0; j<hContours->GetNbinsY(); j++){
      if(hContours->GetBinContent(i+1,j+1) < temp) temp = hContours->GetBinContent(i+1,j+1);
    }

    h1D->SetBinContent(i+1,temp);

    if(min_a_temp > temp){
      min_a_temp = temp;
      *AvNLL = h1D->GetBinContent(i+1);
      *MinVal = h1D->GetBinCenter(i+1);
    }

  }

}

void PTAsimov::SetBestFitPoint() {

  best_all = new TGraph(1);
  best_all->SetPoint(0, s23MinVal, dm23MinVal);

  PrepareBestPoint(best_all,BestFitColor);

}

void PTAsimov::SetAsimovPoint() {

  AsimovRef = new TGraph(1);
  AsimovRef->SetPoint(0, refValue[dm32], refValue[s23]);

  PrepareBestPoint(AsimovRef, AsimovAColor);

}

void PTAsimov::PrintBestPoint() {

  std::cout << "******** BEST FIT ********" << std::endl;
  std::cout << "* sin2_theta23: " << s23MinVal << std::endl;
  std::cout << "* dm2_32: " << dm23MinVal << std::endl;
  std::cout << "**************************" << std::endl;

}

void PTAsimov::CreateHistograms(std::string InputAsimov) {

  SetNumuHistograms();

  Int_t Indexa = AvNLL_BIN_INIT;
  double AvNLLtota = 0.;

  SetSensiTree(InputAsimov, &Indexa, &AvNLLtota);

  FindAvNLLMin(&Indexa, &AvNLLtota);

  CreateNumuContours(&Indexa, &AvNLLtota);

  Create1DYSensi(SA_s23_dm23, SA_dm23_1D, &dm23MinLL, &dm23MinVal);
  Create1DXSensi(SA_s23_dm23, SA_s23_1D, &s23MinLL, &s23MinVal);

}

void PTAsimov::CreateContours1Sigma(double minchi2_asimov, TH2D *hContours) {

  asimov_68 = (TList*)(getContours(minchi2_asimov+2.30,hContours)->Clone());
  joinedasimov_68 = joinContours(asimov_68);

}

void PTAsimov::CreateContours2Sigma(double minchi2_asimov, TH2D *hContours) {

  asimov_90 = (TList*)(getContours(minchi2_asimov+4.61,hContours)->Clone());
  joinedasimov_90 = joinContours(asimov_90);

}

void PTAsimov::CreateLegend() {

  // Make legend
  leg= new TLegend(0.80,0.85,0.98,0.98);
  leg->SetTextFont(132);
  leg->SetLineColor(kBlack);

  // Add 68%/90% lines to legend
  TGraph *g90 = new TGraph(1);
  g90->SetLineColor(ContoursColor);
  g90->SetLineWidth(2);
  TGraph *g68 = new TGraph(1);
  g68->SetLineColor(ContoursColor);
  g68->SetLineWidth(2);
  g68->SetLineStyle(2);
  leg->AddEntry(g90,"90% CL","l");
  leg->AddEntry(g68,"68% CL","l");


}

void PTAsimov::DrawContours(TList *contours, Int_t Color, Int_t LineStyle) {

  TGraph *dummy_g;
  TIter contours_iter(contours);
  while ((dummy_g = (TGraph*)contours_iter())) {
    dummy_g->SetLineColor(Color);
    dummy_g->SetLineStyle(LineStyle);
    dummy_g->SetLineWidth(2);
    dummy_g->Draw("same L");
  }

}



void* PTAsimov::CreateCanvasContours() {

  CreateContours1Sigma(SA_s23_dm23->GetMinimum(), SA_s23_dm23);
  CreateContours2Sigma(SA_s23_dm23->GetMinimum(), SA_s23_dm23);

  // Force Scientific Notation
  TGaxis::SetMaxDigits(3);

  CreateLegend();

  // Make canvas
  c_s23_dm23 = new TCanvas(Form("%s_contours",masterName.c_str()),"contours",800,600);

  // TPad *pad1 = new TPad("pad1","",0,0,1,1);
  // TPad *pad2 = new TPad("pad2","",0,0,1,1);
  //
  // pad1->Draw();
  // pad1->cd();

  // Dummy TH2 for plotting
  TH2D *dummy = new TH2D(Form("%s_%s",
                              masterName.c_str(),
                              "dummy"),
                         "Asimov Sensitivity",
                         nbBins[s23],StartValue[s23],LastValue[s23],
                         nbBins[dm32],StartValue[dm32],LastValue[dm32]);
  if(!masterName.empty()) dummy->SetTitle(masterName.c_str());
  dummy->GetXaxis()->SetTitle("sin^{2}#theta_{23}");
  dummy->GetYaxis()->SetTitle("#Delta m^{2}_{23}");
  dummy->GetYaxis()->SetTitleOffset(1.1);

  dummy->Draw();

  best_all->Draw("P");

  leg->Draw();

  DrawContours(asimov_68, ContoursColor, 2);
  DrawContours(asimov_90, ContoursColor, 1);

  // pad2->SetFrameFillStyle(0);
  // pad2->SetFillStyle(4000);
  // pad2->Draw();
  // pad2->cd();
  //
  // best_all->GetYaxis()->SetRangeUser(StartValue[dm32],LastValue[dm32]);
  // best_all->Draw("AP");
  // best_all->GetXaxis()->SetLimits(StartValue[s23],LastValue[s23]);

  // TLine *lDiag = new TLine(StartValue[s23], StartValue[s23_bar], LastValue[s23], LastValue[s23_bar]);
  // lDiag->SetLineStyle(2);
  // lDiag->SetLineWidth(2.);
  // lDiag->SetLineColor(ARCADIA);
  // lDiag->Draw("SAME");

  c_s23_dm23->Update();
  return c_s23_dm23;

}



TList* PTAsimov::getContours(double contlev, TH2D *h1) {
  // Set contours at the level you specified
  double contourlevel[1] = {contlev};
  h1->SetContour(1, contourlevel);

  // Draw contours
  TCanvas *dummycanv = new TCanvas();
  h1->SetLineColor(kBlue);
  h1->Draw("CONT.LIST");
  dummycanv->Update();
  gPad->Update();

  // Extract contours from TCanvas as TGraphs
  TObjArray *contours_obj = (TObjArray*)(gROOT->GetListOfSpecials()->FindObject("contours"));
  TList *list_cont = (TList*)contours_obj->At(0);

  delete dummycanv;

  return list_cont;
}


TList* PTAsimov::joinContours(TList* list_conts)
{
  // Assuming normally-shaped contours, the possibilities for your contours are:
  // 1) contours are already joined (last point of TGraph = first point of TGraph)
  //    -> don't need to do anything, just return the same TGraph
  // 2) contours are loops that don't join up because they hit a boundary at
  //    dm23 = pi or -pi
  //    These types of contours will have first point and last point both >0 or <0 in y
  //    -> join the loops up (add a point to the TGraph which = first point of TGraph)
  // 3) contours are lines (both ends hit the boundary at dm23 = +/- pi)
  //    These types of contours will have first point > 0 and last point < 0 in y
  //    (or vice-versa)
  //    -> there should always be two of these lines as separate TGraphs -- need to join
  //       them together

  // Make a new TList to return
  TList *closedconts = new TList();

  // TGraphs for lines (if you have to join them)
  TGraph *line1 = NULL;
  TGraph *line2 = NULL;
  bool joinlines = false;

  // Loop through all TGraphs
  TIter next_cont(list_conts);
  TGraph *g;
  while ((g = (TGraph*)next_cont()))
  {
    double first_x, first_y, last_x, last_y;
    int Npoints = g->GetN();
    g->GetPoint(0,first_x,first_y);
    g->GetPoint(Npoints-1,last_x,last_y);

    // Case 1: closed
    if ((first_x == last_x) && (first_y == last_y))
    {
      std::cout<<"case1"<<std::endl;
      closedconts->AddLast(g);
    }

      // Case 2: loop
    else if ((first_y < 0 && last_y < 0) || (first_y > 0 && last_y > 0))
    {
      std::cout<<"case2"<<std::endl;
      g->SetPoint(Npoints,first_x,first_y);
      closedconts->AddLast(g);
    }

      // Case 3: line
      // Assume there are max. two lines that you have to join
    else
    {
      std::cout<<"case3"<<std::endl;
      if (!joinlines) line1 = g;
      else line2 = g;
      joinlines = true;
    }
  } // end loop through vector of TGraphs

  // Now join up the lines (if you need to)
  if (joinlines)
  {
    std::cout << "Joining two separate lines into a single contour" << std::endl;
    int n_line1 = line1->GetN();
    int n_line2 = line2->GetN();
    int npoints_new = n_line1 + n_line2 + 1;
    TGraph *newgraph = new TGraph(npoints_new);

    double x_line1, y_line1, x_line2, y_line2;
    int i_line2=0;

    std::cout<<"done"<<std::endl;

    double x_line1_0, y_line1_0;
    line1->GetPoint(0,x_line1_0,y_line1_0);
    double x_line2_0, y_line2_0;
    line2->GetPoint(0,x_line2_0,y_line2_0);


    for (int ipoint=0; ipoint<npoints_new; ipoint++)
    {
      // First add points from line1
      if (ipoint < n_line1) {
        line1->GetPoint(ipoint,x_line1,y_line1);
        newgraph->SetPoint(ipoint,x_line1,y_line1);
      }
        // Next, add points from line2
      else if (ipoint < (n_line1+n_line2)){
        // Need to check if lines are the same way up
        // x_line1 and y_line1 should already be set from above

        // If you're at the right end of line2, carry on
        if ((y_line1>0.0) == (y_line2_0>0.0)) { // if both >0 or both <0
          i_line2 = ipoint-n_line1;
          line2->GetPoint(i_line2,x_line2,y_line2);
          newgraph->SetPoint(ipoint,x_line2,y_line2);
        }

          // If you're at the wrong end of line2, need to go in reverse
        else {
          i_line2 = n_line2 - (ipoint-n_line1);
          line2->GetPoint(i_line2,x_line2,y_line2);
          newgraph->SetPoint(ipoint,x_line2,y_line2);
        }
      }

      // Finally, join them up
      if (ipoint == npoints_new-1) {
        newgraph->SetPoint(ipoint,x_line1_0,y_line1_0);
      }
    }
    closedconts->AddLast(newgraph);
  }

  return closedconts;
}



void PTAsimov::WriteOutputFile() {

  FOut = new TFile(Form("outputs/%s.root",masterName.c_str()),"RECREATE");
  FOut->cd();
  SA_s23_dm23->Write();
  SA_dm23_1D->Write();
  SA_s23_1D->Write();

  c_s23_dm23->Write();
  FOut->Close();

  TString *canvname = new TString("t23_dm23");
  canvname->Append(".pdf");
  canvname->Prepend("_");
  canvname->Prepend(masterName.c_str());
  canvname->Prepend("pdf/");
  c_s23_dm23->Print(canvname->Data());

}
void PTAsimov::CreateGraphFromTH1(TH1 *h) {

  auto NbBins = h->GetNbinsX();

  gr_dm23_1D = new TGraph();

  for(auto i=0; i<NbBins; i++){

    gr_dm23_1D->SetPoint(i,h->GetBinCenter(i+1),h->GetBinContent(i+1));

  }

}

double PTAsimov::Find1Sigma(double *minus, double* plus) {

  if(!gr_dm23_1D) {
    if(!SA_dm23_1D){
      exit(EXIT_FAILURE);
    } else{
      CreateGraphFromTH1(SA_dm23_1D);
    }
  }

  int nbStep = 1e4;
  auto nbPts = gr_dm23_1D->GetN();

  double xMin, yMin;
  double xMax, yMax;

  gr_dm23_1D->GetPoint(0,xMin,yMin);
  gr_dm23_1D->GetPoint(nbPts-1,xMax,yMax);

  double step = (xMax - xMin)/nbStep;

  double step_xMin2min = (dm23MinVal - xMin)/nbStep;
  double step_min2xMax = (xMax - dm23MinVal)/nbStep;

  double minus1=0;
  double plus1=0;

  std::cout << "PTAsimov::Find1Sigma()" << std::endl;
  for(auto i=0; i<nbStep; i++) {
    auto pt = xMin + i*step_xMin2min;
    auto y = gr_dm23_1D->Eval(pt,0,"");

    if(y < 1.001 && y > 0.999) {
      std::cout << "pt: " << pt << " y: " << y << std::endl;
      minus1 = pt;
      break;
    }
  }

  for(auto i=0; i<nbStep; i++) {
    auto pt = dm23MinVal + i*step_min2xMax;
    auto y = gr_dm23_1D->Eval(pt,0,"");

    if(y < 1.001 && y > 0.999) {
      std::cout << "pt: " << pt << " y: " << y << std::endl;
      plus1 = pt;
      break;
    }
  }

  std::cout << "PTAsimov::Find1Sigma()" << std::endl;
  std::cout << "1sigma: " << plus1-minus1 << std::endl;
  std::cout << "bestfit: " << dm23MinVal << std::endl;
  std::cout << "mid: " << minus1 + (plus1-minus1)/2 <<  std::endl;

  *minus=minus1;
  *plus=plus1;

  return plus1-minus1;

  // for(auto i=0; i<nbStep; i++){
  //   auto pt = xMin + i*step;
  //   auto y = gr_dm23_1D->Eval(pt,0,"");
  //
  //   if(y < 1.001 && y > 0.999){
  //     std::cout << "pt: " << pt << " y: " << y << std::endl;
  //   }
  // }


}

double PTAsimov::FindSmoothMin() {

  if(!gr_dm23_1D) {
    std::cout << "PTAsimov::FindSmoothMin() no gr_dm23_1D" << std::endl;
    if(!SA_dm23_1D){
      std::cout << "PTAsimov::FindSmoothMin() no SA_dm23_1D" << std::endl;
      exit(EXIT_FAILURE);
    }

    CreateGraphFromTH1(SA_dm23_1D);

  }

  int nbStep = 1e6;
  auto nbPts = gr_dm23_1D->GetN();

  double xMin, yMin;
  double xMax, yMax;

  gr_dm23_1D->GetPoint(0,xMin,yMin);
  gr_dm23_1D->GetPoint(nbPts-1,xMax,yMax);

  double step = (xMax - xMin)/nbStep;

  std::cout << "PTAsimov::FindSmoothMin()" << std::endl;
  for(int i=0; i<nbStep; i++){
    auto pt = xMin + i*step;
    auto y = gr_dm23_1D->Eval(pt);

    if(y < 1e-6){
      std::cout << "pt: " << pt << " y: " << y << std::endl;
    }
  }

  return 0;
}

