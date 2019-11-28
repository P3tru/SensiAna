void PlotEthetaDistrib(std::string filename, std::string distribname = "pdf_numu_etheta_im"){

  gStyle->SetOptStat(0);
  gStyle->SetPalette(56);

  // There's 8 int channel for numu RHC and FHC
  const int nInt = 8;

  TH2D* hEthetaPerInt[nInt];

  // Open file
  TFile *f = TFile::Open(filename.c_str());

  // Plot in different canvas
  TCanvas *c1;

  // Access each TH2
  for(int iInt = 0; iInt<nInt; iInt++){
    // Recover each histogram
    hEthetaPerInt[iInt] = (TH2D*)f->Get(Form("%s%d", distribname.c_str(), iInt));

    // Create legend axis
    hEthetaPerInt[iInt]->SetTitle("");
    hEthetaPerInt[iInt]->GetXaxis()->SetTitle("E_{rec} (GeV)");
    hEthetaPerInt[iInt]->GetYaxis()->SetTitle("#theta (deg)");
    hEthetaPerInt[iInt]->GetYaxis()->SetTitleOffset(1.2);

    if (hEthetaPerInt[iInt]->GetEntries() == 0) continue;

    // Create individual canvas
    c1 = new TCanvas(Form("c%s%d", distribname.c_str(), iInt),
		     Form("c%s%d", distribname.c_str(), iInt),
		     1600,
		     1200);
    c1->SetGrid();

    hEthetaPerInt[iInt]->Draw("COLZ");    
    hEthetaPerInt[iInt]->GetXaxis()->SetRangeUser(0,1.25);

    c1->Print(Form("c%s%d.pdf", distribname.c_str(), iInt), "pdf");
  }
  

}
