//
// Created by zsoldos on 11/19/19.
//

void SuperposeSpectrum(std::string fDataname, std::string f2name="", std::string h2name=""){

  gStyle->SetOptStat(0);
  gStyle->SetPalette(56);

  TFile *fData = TFile::Open(fDataname.c_str());

  static const Int_t N_NUMU_EREC_BINS = 73;
  static const Int_t N_T_BINS = 15;

  const double AngleEdges[N_T_BINS+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,180};
  //Edges of numu Erec bins
  const double ErecEdges[N_NUMU_EREC_BINS+1]=
	  {0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50,
	   0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.00,
	   1.05, 1.10, 1.15, 1.20, 1.25, 1.30, 1.35, 1.40, 1.45, 1.50,
	   1.55, 1.60, 1.65, 1.70, 1.75, 1.80, 1.85, 1.90, 1.95, 2.00,
	   2.05, 2.10, 2.15, 2.20, 2.25, 2.30, 2.35, 2.40, 2.45, 2.50,
	   2.55, 2.60, 2.65, 2.70, 2.75, 2.80, 2.85, 2.90, 2.95, 3.00,
	   3.25, 3.50, 3.75, 4.00,
	   4.50, 5.00, 5.50, 6.00,
	   7.00, 8.00, 9.00, 10.00,
	   30.00
	  };

  TH2D *hDataRebin = new TH2D("hDataRebin","Erec-theta distribution for data #nu_{#mu} FHC+RHC",
							  N_NUMU_EREC_BINS, ErecEdges,
							  N_T_BINS, AngleEdges);
  hDataRebin->GetXaxis()->SetTitle("Erec (GeV)");
  hDataRebin->GetYaxis()->SetTitle("Angle (deg)");

  std::vector<double> *rErec_numu=new std::vector<double>();
  std::vector<double> *rt_numu=new std::vector<double>();

  TTree *ToyXp=(TTree*)fData->Get("Data_Tree");

  ToyXp->SetBranchAddress("Erec_numu",&rErec_numu);
  ToyXp->SetBranchAddress("t_numu",&rt_numu);

  int ExpNb = 0;
  assert(ExpNb<ToyXp->GetEntries());
  ToyXp->GetEntry(ExpNb);

  for(unsigned int i = 0; i < rErec_numu->size() ; i++){
	hDataRebin->Fill(rErec_numu->at(i),rt_numu->at(i));
  }

  TCanvas *c1;

  c1 = new TCanvas("c1","c1",1600,1200);
  hDataRebin->Draw("COLZ");
  hDataRebin->GetXaxis()->SetRangeUser(0,6);
  TH1D *hDataProj = hDataRebin->ProjectionX();
  hDataProj->GetXaxis()->SetTitle("Erec (GeV)");
  hDataProj->GetYaxis()->SetTitle("Number of Events");
  hDataProj->SetLineColor(kBlue-4);
  hDataProj->SetLineWidth(2);

  TLegend *leg = new TLegend(0.22,0.82,0.90,0.90);
  leg->AddEntry(hDataProj,"Data");

  if(!f2name.empty() || !h2name.empty()) {
	TFile *f2 = TFile::Open(f2name.c_str());
	c1 = new TCanvas("c2","c2",1600,1200);
	TH2D *h2 = (TH2D*)f2->Get(h2name.c_str());
	h2->Draw("COLZ");

	std::cout << h2->GetEntries() << std::endl;

	h2->GetXaxis()->SetRangeUser(0,6);
	TH1D *hBFProj = h2->ProjectionX();
	hBFProj->SetLineColor(kRed-4);
	hBFProj->SetLineWidth(2);

	hBFProj->GetXaxis()->SetTitle("Erec (GeV)");
	hBFProj->GetYaxis()->SetTitle("Number of Events");


	c1 = new TCanvas("c3","c3",1600,1200);
	hBFProj->Draw("ESAME");
	hDataProj->Draw("SAME");
	hBFProj->SetTitle("Projected Erec distribution for data and atm best fit");

	leg->AddEntry(hBFProj,"#Deltam^{2}_{32}=2.471e-3, sin^{2}#theta_{23}=0.510, #Delta#bar{m}^{2}_{32}=2.500e-3, sin^{2}#bar{#theta}_{23}=0.431");
	leg->Draw();


  }

}