{
  TFile *fErec = new TFile("withEb/Erec/nu_beam/numu_ntrw12.root","READ");
  TFile *fEtheta = new TFile("withEb/Etheta/nu_beam/numu_ntrw12.root","READ");

  TCanvas *c1 = new TCanvas("c1","c1",800,600);

  TGraph *grErec = (TGraph*)fErec->Get("gr_rw_Erec_im0_enu0_binx7");
  grErec->SetLineColor(kBlack);
  grErec->SetMarkerColor(kBlack);

  grErec->SetLineWidth(2);
  grErec->SetLineStyle(2);

  grErec->SetTitle("RF Erec_im0_enu0_binx7; Eb dev; nu_mubar evts");
  grErec->Draw();

  TGraph *grET = (TGraph*)fEtheta->Get("gr_rw_Erec_im0_enu0_binx7");
  grET->SetLineColor(kRed);
  grET->SetMarkerColor(kRed);

  grET->SetLineWidth(2);
  grET->SetLineStyle(2);
  grET->Draw("SAME");

  TLegend *leg = new TLegend(0.699248,0.911458,0.997494,0.991319);
  leg->AddEntry(grErec,"Erec only Koga-san","pl");
  leg->AddEntry(grET,"Erec from E-theta integral","pl");
  leg->Draw();

  const int nT = 15;
  TGraph *grEtheta[nT];

  

  for(int i = 1; i<nT; i++){
    grEtheta[i] = (TGraph*)fEtheta->Get(Form("gr_rw_etheta_im0_enu0_binx7_biny%d",i));
    grEtheta[i]->SetLineColor(i);
    grEtheta[i]->SetMarkerColor(i);
    grEtheta[i]->Draw("SAME");
  }


}
