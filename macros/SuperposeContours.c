//
// Created by zsoldos on 11/18/19.
//

TList* getContours(double contlev, TH2D *h1) {
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


TList* joinContours(TList* list_conts)
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

void DrawContours(TList *contours, Int_t Color, Int_t LineStyle) {

  TGraph *dummy_g;
  TIter contours_iter(contours);
  while ((dummy_g = (TGraph*)contours_iter())) {
	dummy_g->SetLineColor(Color);
	dummy_g->SetLineStyle(LineStyle);
	dummy_g->SetLineWidth(2);
	dummy_g->Draw("same L");
  }

}

#define PI 3.14159265358979312
enum OscPar{
  s13=0,      //sin2(2theta13)
  delta=1,    //delta_CP
  dm32=2,     //Delta m2 32 in nu mode
  s23=3,      //sin2(theta23) in nu mode
  dm32_bar=4, //Delta m2 32 in nubar mode
  s23_bar=5   //sin2(theta23) in nubar mode
};

#define AvNLL_INFINITY 999999.
#define AvNLL_BIN_INIT -1


void GetMinTH2(TH2D *h, double *thetaMin, double *dmMin){

  double min_a_temp = AvNLL_INFINITY;

  for(int i=0; i<h->GetNbinsY(); i++) {

	double temp = AvNLL_INFINITY;

	for (int j = 0; j < h->GetNbinsX(); j++) {
	  if (h->GetBinContent(j + 1, i + 1) < temp){
		temp = h->GetBinContent(j + 1, i + 1);
		*thetaMin = h->GetXaxis()->GetBinCenter(j);
		*dmMin = h->GetYaxis()->GetBinCenter(i);;
	  }
	}

  }

  std::cout << "thetaMin: " << *thetaMin << std::endl;
  std::cout << "dmMin: " << *dmMin << std::endl;

}

void SuperposeContours(std::string f1name, std::string h1name, std::string f2name="", std::string h2name=""){

  static const Int_t N_OSC_PAR =  6;//number of used oscillation parameters

  int    nbBins[N_OSC_PAR]; // nbBins for each parameter
  double refValue[N_OSC_PAR];
  double StartValue[N_OSC_PAR];
  double LastValue[N_OSC_PAR];

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

  TFile *f1 = TFile::Open(f1name.c_str());
  TH2D *h1 = (TH2D*)f1->Get(h1name.c_str());

  TList *asimov_68 = (TList*)(getContours(h1->GetMinimum()+2.30,h1)->Clone());
  TList *joinedasimov_68 = joinContours(asimov_68);

  TList *asimov_90 = (TList*)(getContours(h1->GetMinimum()+4.61,h1)->Clone());
  TList *joinedasimov_90 = joinContours(asimov_90);

  TCanvas *c1;
//  TPad *pad1 = new TPad("pad1","",0,0,1,1);

  c1 = new TCanvas("c1","c1",1600,1200);
//  pad1->Draw();
//  pad1->cd();
  c1->SetGrid();
  TGaxis::SetMaxDigits(3);

  TH2D *dummy = new TH2D(Form("%s_%s",
							  "test",
							  "dummy"),
						 "Asimov Sensitivity",
						 nbBins[s23],StartValue[s23],LastValue[s23],
						 nbBins[dm32],StartValue[dm32],LastValue[dm32]);
  dummy->SetTitle("Atmospheric #nu_{#mu} parameters");
  dummy->GetXaxis()->SetTitle("sin^{2}#theta_{23}");
  dummy->GetYaxis()->SetTitle("#Delta m^{2}_{23}");
  dummy->GetYaxis()->SetTitleOffset(1.1);

  dummy->SetStats(false);

  dummy->Draw();

  DrawContours(asimov_68, kBlue-4, 2);
  DrawContours(asimov_90, kBlue-4, 1);

  TMultiGraph *mgBestFit = new TMultiGraph();
  TGraph *grBestFitDummy = (TGraph*)f1->Get("gr_BF");
  grBestFitDummy->SetMarkerColor(kBlue-4);
  grBestFitDummy->SetMarkerSize(1.5);
  grBestFitDummy->SetMarkerStyle(kFullCrossX);

  double x,y;
  grBestFitDummy->GetPoint(0,x,y);
  std::cout << x << " " << y << std::endl;
  mgBestFit->Add(grBestFitDummy,"p");


//  h1->GetYaxis()->SetRangeUser(2.2e-3,2.8e-3);

  if(!f2name.empty() || !h2name.empty()){
	TFile *f2 = TFile::Open(f2name.c_str());
	TH2D *h2 = (TH2D*)f2->Get(h2name.c_str());

	TList *asimov_68 = (TList*)(getContours(h2->GetMinimum()+2.30,h2)->Clone());
	TList *joinedasimov_68 = joinContours(asimov_68);

	TList *asimov_90 = (TList*)(getContours(h2->GetMinimum()+4.61,h2)->Clone());
	TList *joinedasimov_90 = joinContours(asimov_90);

	DrawContours(asimov_68, kRed-4, 2);
	DrawContours(asimov_90, kRed-4, 1);

	TGraph *grBestFitDummy = (TGraph*)f2->Get("gBF_NH");
	grBestFitDummy->SetMarkerColor(kRed-4);
	grBestFitDummy->SetMarkerSize(1.5);
	grBestFitDummy->SetMarkerStyle(kFullCrossX);

	double x,y;
	grBestFitDummy->GetPoint(0,x,y);
	std::cout << x << " " << y << std::endl;
	mgBestFit->Add(grBestFitDummy,"p");

  }


  // Make legend
  TLegend *leg= new TLegend(0.40,0.72,0.90,0.90);
  leg->SetTextFont(132);
  leg->SetLineColor(kBlack);

  // Add 68%/90% lines to legend
  TGraph *g90 = new TGraph(1);
  g90->SetLineColor(kBlue-4);
  g90->SetLineWidth(2);
  TGraph *g68 = new TGraph(1);
  g68->SetLineColor(kBlue-4);
  g68->SetLineWidth(2);
  g68->SetLineStyle(2);
  leg->AddEntry(g90,"1R#mu FHC+RHC 90% CL","l");
  leg->AddEntry(g68,"1R#mu FHC+RHC 68% CL","l");

  g90 = new TGraph(1);
  g90->SetLineColor(kRed-4);
  g90->SetLineWidth(2);
  g68 = new TGraph(1);
  g68->SetLineColor(kRed-4);
  g68->SetLineWidth(2);
  g68->SetLineStyle(2);
  leg->AddEntry(g90,"1Re and 1R#mu FHC+RHC 90% CL","l");
  leg->AddEntry(g68,"1Re and 1R#mu FHC+RHC 68% CL","l");

  TPad *pad2 = new TPad("pad2","",0,0,1,1);
  pad2->SetFrameFillStyle(0);
  pad2->SetFillStyle(4000);
  pad2->Draw();
  pad2->cd();
//
  dummy = new TH2D("dummymg",
				   "",
				   nbBins[s23],StartValue[s23],LastValue[s23],
				   nbBins[dm32],StartValue[dm32],LastValue[dm32]);

  dummy->SetStats(false);

  dummy->Draw();

  mgBestFit->Draw("AP");
  mgBestFit->GetYaxis()->SetRangeUser(2.2e-3,3.0e-3);
  mgBestFit->GetXaxis()->SetLimits(0.3,0.7);

  leg->Draw();


}