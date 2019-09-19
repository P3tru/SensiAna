//
// Created by zsoldos on 02/12/18.
//

#ifndef ANALYSIS_PTASIMOV_H
#define ANALYSIS_PTASIMOV_H

#include <TChain.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TFile.h>
#include <TGraph.h>
#include <TColor.h>

#include "Utils.h"

#define PI 3.14159265358979312
static const Int_t N_OSC_PAR =  6;//number of used oscillation parameters
const char OscParNames[N_OSC_PAR][64]={
    "sin2_2theta13",
    "delta_CP",
    "dm2_32",
    "sin2_theta23",
    "dm2_32bar",
    "sin2_theta23_bar"
};

enum OscPar{
  s13=0,      //sin2(2theta13)
  delta=1,    //delta_CP
  dm32=2,     //Delta m2 32 in nu mode
  s23=3,      //sin2(theta23) in nu mode
  dm32_bar=4, //Delta m2 32 in nubar mode
  s23_bar=5   //sin2(theta23) in nubar mode
};

class PTAsimov {

 protected:

  TChain *Sensia;
  TChain *Sensih;

  int    nbBins[N_OSC_PAR]; // nbBins for each parameter
  double refValue[N_OSC_PAR];
  double StartValue[N_OSC_PAR];
  double LastValue[N_OSC_PAR];

  TH2D    *SA_s23_dm23;
  TCanvas *c_s23_dm23;

  TH1D *SA_dm23_1D;
  TH1D *SA_s23_1D;

  TGraph *gr_dm23_1D;

  double globalMinLL;
  int globalMinBin;

  double dm23MinLL;
  int dm23MinBin;
  double dm23MinVal;

  double s23MinLL;
  int s23MinBin;
  double s23MinVal;

  TList *asimov_68;
  TList *asimov_90;

  TList *joinedasimov_68;
  TList *joinedasimov_90;

  Int_t ContoursColor;
  Int_t BestFitColor;
  Int_t AsimovAColor;

  std::string masterName;

  TFile *FOut;

  TGraph *best_all;
  TGraph *AsimovRef;

  TLegend *leg;

 public:

  ////////////////////////////////
  // CONSTRUCTOR AND DESTRUCTOR //
  ////////////////////////////////

  PTAsimov();
  virtual ~PTAsimov();

  ////////////////////
  // PUBLIC METHODS //
  ////////////////////

  void InitializeDefaultParams();
  void InitializeParamsFromSensiHeader();

  void SetNumuHistograms();

  void SetSensiTree(std::string inputName, Int_t *Indexa, double *AvNLLtota);

  void FindAvNLLMin(Int_t *Indexa, double *AvNLLtota);

  void CreateHistograms(std::string InputAsimov);
  void CreateNumuContours(Int_t *Indexa, double *AvNLLtota);
  void Create1DXSensi(TH2D *hContours, TH1D* h1D, double *AvNLL, double *MinVal);
  void Create1DYSensi(TH2D *hContours, TH1D* h1D, double *AvNLL, double *MinVal);

  void SetBestFitPoint();
  void SetAsimovPoint();

  void PrintBestPoint();

  void CreateContours1Sigma(double minchi2_asimov, TH2D *hContours);
  void CreateContours2Sigma(double minchi2_asimov, TH2D *hContours);

  void CreateLegend();

  void DrawContours(TList *contours, Int_t Color, Int_t LineStyle);

  void* CreateCanvasContours();
  TList* getContours(double contlev, TH2D *h1);
  TList* joinContours(TList* list_conts);

  void CreateGraphFromTH1(TH1 *h);

  double Find1Sigma(double *minus=NULL, double* plus=NULL);
  double FindSmoothMin();


  void WriteOutputFile();

  ///////////////////////
  // GETTER AND SETTER //
  ///////////////////////

  TH2D *getSA_s23_dm23() const {
    return SA_s23_dm23;
  }
  void setSA_s23_dm23(TH2D *SA_s23_dm23) {
    PTAsimov::SA_s23_dm23 = SA_s23_dm23;
  }
  TCanvas *getC_s23_dm23() const {
    return c_s23_dm23;
  }
  void setC_s23_dm23(TCanvas *c_s23_dm23) {
    PTAsimov::c_s23_dm23 = c_s23_dm23;
  }

  TH1D *getSA_dm23_1D() const {
    return SA_dm23_1D;
  }
  void setSA_dm23_1D(TH1D *SA_dm23_1D) {
    PTAsimov::SA_dm23_1D = SA_dm23_1D;
  }
  TH1D *getSA_s23_1D() const {
    return SA_s23_1D;
  }
  void setSA_s23_1D(TH1D *SA_s23_1D) {
    PTAsimov::SA_s23_1D = SA_s23_1D;
  }

  TGraph *getBest_all() const {
    return best_all;
  }
  void setBest_all(TGraph *best_all) {
    PTAsimov::best_all = best_all;
  }
  TGraph *getAsimovRef() const {
    return AsimovRef;
  }
  void setAsimovRef(TGraph *AsimovRef) {
    PTAsimov::AsimovRef = AsimovRef;
  }

  const std::string &getMasterName() const {
    return masterName;
  }
  void setMasterName(const std::string &masterName) {
    PTAsimov::masterName = masterName;
  }

  const int *getNbBins() const {
    return nbBins;
  }
  const double *getRefValue() const {
    return refValue;
  }
  const double *getStartValue() const {
    return StartValue;
  }
  const double *getLastValue() const {
    return LastValue;
  }

  TList *getAsimov_68() const {
    return asimov_68;
  }
  void setAsimov_68(TList *asimov_68) {
    PTAsimov::asimov_68 = asimov_68;
  }
  TList *getAsimov_90() const {
    return asimov_90;
  }
  void setAsimov_90(TList *asimov_90) {
    PTAsimov::asimov_90 = asimov_90;
  }

  Int_t getContoursColor() const {
    return ContoursColor;
  }
  void setContoursColor(Int_t ContoursColor) {
    PTAsimov::ContoursColor = ContoursColor;
  }
  Int_t getBestFitColor() const {
    return BestFitColor;
  }
  void setBestFitColor(Int_t BestFitColor) {
    PTAsimov::BestFitColor = BestFitColor;
  }
  Int_t getAsimovAColor() const {
    return AsimovAColor;
  }
  void setAsimovAColor(Int_t AsimovAColor) {
    PTAsimov::AsimovAColor = AsimovAColor;
  }
  double getDm23MinVal() const {
    return dm23MinVal;
  }
  void setDm23MinVal(double dm23MinVal) {
    PTAsimov::dm23MinVal = dm23MinVal;
  }
  double getS23MinVal() const {
    return s23MinVal;
  }
  void setS23MinVal(double s23MinVal) {
    PTAsimov::s23MinVal = s23MinVal;
  }
  TGraph *getGrDm231D() const {
    return gr_dm23_1D;
  }
  void setGrDm231D(TGraph *grDm231D) {
    gr_dm23_1D = grDm231D;
  }

};

#endif //ANALYSIS_PTASIMOV_H
