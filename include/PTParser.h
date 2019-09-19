//
// Created by zsoldos on 02/12/18.
//

#ifndef ANALYSIS_PTPARSER_H
#define ANALYSIS_PTPARSER_H

#include <vector>
#include <string>

#include <TApplication.h>
#include <TColor.h>

#include "Utils.h"

typedef std::map< std::string, std::vector<std::string> > AnalysisMap;
struct analysismap{
  analysismap(AnalysisMap::iterator& it)
      : anaName(it->first), fileList(it->second) {}
  const std::string anaName;
  std::vector<std::string>& fileList;
};

class PTParser {

 protected:

  // Create TApp
  TApplication *theApp;

  // Counter nb of Files
  std::map<std::string, int> nFiles;

  // Input and Output filenames
  // std::vector< std::vector<std::string> > FInAsimov;
  std::map< std::string, std::vector<std::string> > FInAsimov;
  std::string FOut;

  // FLAG to plot contours on same Canvas
  bool PlotOnSameCanvas;
  // Name of same canvas;
  std::string NameSameCanvas;

  // Color for plots
  Int_t ContoursColor;
  Int_t BestFitColor;
  Int_t AsimovAColor;


 public:

  ////////////////////////////////
  // CONSTRUCTOR AND DESTRUCTOR //
  ////////////////////////////////

  explicit PTParser(TApplication *App);
  // Destructor
  ~PTParser();

  ////////////////////
  // PUBLIC METHODS //
  ////////////////////

  void LaunchApp();

  void ShowUsage(std::string name);

  void ProcessArgs();

  ///////////////////////
  // GETTER AND SETTER //
  ///////////////////////

  const std::map<std::string, int> &getNFiles() const {
    return nFiles;
  }
  void setNFiles(const std::map<std::string, int> &nFiles) {
    PTParser::nFiles = nFiles;
  }

  const std::map<std::string, std::vector<std::string>> &getFInAsimov() const {
    return FInAsimov;
  }
  void setFInAsimov(const std::map<std::string, std::vector<std::string>> &FInAsimov) {
    PTParser::FInAsimov = FInAsimov;
  }

  const std::string &getFOut() const {
    return FOut;
  }
  void setFOut(const std::string &FOut) {
    PTParser::FOut = FOut;
  }
  bool isPlotOnSameCanvas() const {
    return PlotOnSameCanvas;
  }
  void setPlotOnSameCanvas(bool PlotOnSameCanvas) {
    PTParser::PlotOnSameCanvas = PlotOnSameCanvas;
  }
  const std::string &getNameSameCanvas() const {
    return NameSameCanvas;
  }
  void setNameSameCanvas(const std::string &NameSameCanvas) {
    PTParser::NameSameCanvas = NameSameCanvas;
  }
  Int_t getContoursColor() const {
    return ContoursColor;
  }
  void setContoursColor(Int_t ContoursColor) {
    PTParser::ContoursColor = ContoursColor;
  }
  Int_t getBestFitColor() const {
    return BestFitColor;
  }
  void setBestFitColor(Int_t BestFitColor) {
    PTParser::BestFitColor = BestFitColor;
  }
  Int_t getAsimovAColor() const {
    return AsimovAColor;
  }
  void setAsimovAColor(Int_t AsimovAColor) {
    PTParser::AsimovAColor = AsimovAColor;
  }
};

class BiasParser {

 protected:

  // Create TApp
  TApplication *theApp;

  std::string cAsimov;
  std::string cFDS;

 public:

  ////////////////////////////////
  // CONSTRUCTOR AND DESTRUCTOR //
  ////////////////////////////////

  explicit BiasParser(TApplication *App);
  // Destructor
  ~BiasParser();

  ////////////////////
  // PUBLIC METHODS //
  ////////////////////

  void LaunchApp();

  void ShowUsage(std::string name);

  void ProcessArgs();

  ///////////////////////
  // GETTER AND SETTER //
  ///////////////////////


  const std::string &getCAsimov() const {
    return cAsimov;
  }
  void setCAsimov(const std::string &cAsimov) {
    BiasParser::cAsimov = cAsimov;
  }
  const std::string &getCfds() const {
    return cFDS;
  }
  void setCfds(const std::string &cFds) {
    cFDS = cFds;
  }

};

#endif //ANALYSIS_PTPARSER_H
