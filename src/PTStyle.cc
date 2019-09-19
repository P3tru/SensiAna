//
// Created by zsoldos on 02/12/18.
//

#include "PTStyle.h"

PTStyle::PTStyle(const std::string &styleName, const std::string &styleOptName)
    : styleName(styleName), styleOptName(styleOptName) {

  style = new TStyle(styleName.c_str(),styleOptName.c_str());

}

PTStyle::PTStyle() {

  styleName = "T2K";
  styleOptName = "T2K approved plots style";

  style = new TStyle(styleName.c_str(),styleOptName.c_str());

  SetT2KStyleVariables();

}

PTStyle::~PTStyle() {

  delete style;

}

void PTStyle::SetT2KStyleVariables() {

  // use plain black on white colors
  style->SetFrameBorderMode(0);
  style->SetCanvasBorderMode(0);
  style->SetPadBorderMode(0);
  style->SetPadColor(0);
  style->SetCanvasColor(0);
  style->SetStatColor(0);
  style->SetFillColor(0);
  style->SetLegendBorderSize(1);

  // set the paper & margin sizes
  style->SetPaperSize(20,26);
  style->SetPadTopMargin(0.05);
  style->SetPadRightMargin(0.15); //0.05
  style->SetPadBottomMargin(0.16);
  style->SetPadLeftMargin(0.13);

  // use large Times-Roman fonts
  style->SetTextFont(132);
  style->SetTextSize(0.08);
  style->SetNdivisions(5,"x");
  style->SetNdivisions(10,"y");
  style->SetLabelFont(132,"x");
  style->SetLabelFont(132,"y");
  style->SetLabelFont(132,"z");
  style->SetLabelSize(0.06,"x");
  style->SetTitleSize(0.06,"x");
  style->SetLabelSize(0.06,"y");
  style->SetTitleSize(0.06,"y");
  style->SetLabelSize(0.06,"z");
  style->SetTitleSize(0.06,"z");
  style->SetLabelFont(132,"t");
  style->SetTitleFont(132,"x");
  style->SetTitleFont(132,"y");
  style->SetTitleFont(132,"z");
  style->SetTitleFont(132,"t");
  style->SetTitleFillColor(0);
  style->SetTitleX(0.25);
  style->SetTitleFontSize(0.08);
  style->SetTitleFont(132,"pad");

  style->SetPadGridX(true);
  style->SetPadGridY(true);

  // use bold lines and markers
  //  style->SetMarkerStyle(20);
  style->SetHistLineWidth(1.85);
  style->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars and y error bar caps
  //  style->SetErrorX(0.001);

  // do not display any of the standard histogram decorations
  style->SetOptTitle(0);
  style->SetOptStat(0);
  style->SetOptFit(0);

  // put tick marks on top and RHS of plots
  style->SetPadTickX(1);
  style->SetPadTickY(1);

  //style->SetPalette(1,0);  // use the nice red->blue palette
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue,
                                   NCont);
  style->SetNumberContours(NCont);

  // End of definition of style
}

void PTStyle::SetCustomStyleVariables() {

  // use plain black on white colors
  style->SetFrameBorderMode(0);
  style->SetCanvasBorderMode(0);
  style->SetPadBorderMode(0);
  style->SetPadColor(0);
  style->SetCanvasColor(0);
  style->SetStatColor(0);
  style->SetFillColor(0);
  style->SetLegendBorderSize(1);

  // set the paper & margin sizes
  style->SetPaperSize(20,26);
  // style->SetPadTopMargin(0.05);
  style->SetPadRightMargin(0.15); //0.05
  // style->SetPadBottomMargin(0.16);
  style->SetPadLeftMargin(0.13);


  style->SetPadGridX(true);
  style->SetPadGridY(true);

  // use bold lines and markers
  //  style->SetMarkerStyle(20);
  style->SetHistLineWidth(1.85);
  style->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars and y error bar caps
  //  style->SetErrorX(0.001);

  // do not display any of the standard histogram decorations
  // style->SetOptTitle(0);
  style->SetOptStat(0);
  style->SetOptFit(0);

  // put tick marks on top and RHS of plots
  style->SetPadTickX(1);
  style->SetPadTickY(1);

  //style->SetPalette(1,0);  // use the nice red->blue palette
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue,
                                   NCont);
  style->SetNumberContours(NCont);

  // End of definition of style
}
