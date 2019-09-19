//
// Created by zsoldos on 02/12/18.
//

#include <iostream>

#include "PTParser.h"

PTParser::PTParser(TApplication *App){

  theApp = App;
  nFiles.clear();
  FInAsimov.clear();
  FOut.clear();
  PlotOnSameCanvas=false;
  NameSameCanvas.clear();
  ContoursColor=0;
  BestFitColor=0;
  AsimovAColor=0;


}

PTParser::~PTParser() {

  delete theApp;

}

void PTParser::LaunchApp() {

  PTParser::ProcessArgs();

}

void PTParser::ShowUsage(std::string name) {

  std::cerr << "Usage: " << std::endl
            << "\t" << name << " <Options> -n (--name) [FILES] -n (--name) [FILES2] ..." << std::endl
            << "Options:" << std::endl
            << std::left  << std::setw(30) << "\t-h (--help)"
            << std::left << std::setw(50)
            << "Show this help message" << std::endl
            << std::left  << std::setw(30) << "\t-o (--out)"
            << std::left << std::setw(50)
            << "Output filename" << std::endl
            << std::left  << std::setw(30) << "\t-s (--same)"
            << std::left << std::setw(50)
            << "Plot all sources on same canvas" << std::endl
            << std::left  << std::setw(30) << "\t-sn (--samename) NAME"
            << std::left << std::setw(50)
            << "Set name for same Canvas" << std::endl
            << std::left  << std::setw(30) << "\t-cc (--contcol) COLOR"
            << std::left << std::setw(50)
            << "Set color for contours" << std::endl
            << std::left  << std::setw(30) << "\t-bc (--bestcol) COLOR"
            << std::left << std::setw(50)
            << "Set color for best point" << std::endl
            << std::left  << std::setw(30) << "\t-rc (--refcol) COLOR"
            << std::left << std::setw(50)
            << "Set color for Asimov point" << std::endl
            // << std::left  << std::setw(30) << "\t-n (--name) NAME [FILES] -e"
            // << std::left << std::setw(50)
            // << "Specify input files associated to analysis NAME" << std::endl
            << std::endl;

  std::cerr << "Color available: " << std::endl
            << " " << "0: Blue" << std::endl
            << " " << "1: Red" << std::endl
            << " " << "2: Teal" << std::endl
            << " " << "3: Pink" << std::endl
            << " " << "4: Brown" << std::endl
            << " " << "5: Violet" << std::endl;
  std::cerr << "(You can add more colors in include/Utils.h)" << std::endl;

}

void PTParser::ProcessArgs() {

  // Reading user input parameters
  if (theApp->Argc() < 2) {
    PTParser::ShowUsage(theApp->Argv(0));
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < theApp->Argc(); i++) {

    std::string arg = theApp->Argv(i);

    if ((arg == "-h") || (arg == "--help")) {

      PTParser::ShowUsage(theApp->Argv(0));
      exit(EXIT_SUCCESS);

    } else if ((arg == "-n") || (arg == "--name")) {

      std::string name = theApp->Argv(++i);
      nFiles[name]=0;
      std::cout << "i: " << i << " name: " << name << std::endl;

      for(int j = i+1 ; j < theApp->Argc(); j++){
        if( strcmp(theApp->Argv(j), "") == 0 ||
            strcmp(theApp->Argv(j), "-n") == 0 ||
            strcmp(theApp->Argv(j), "--name") == 0 ||
            strcmp(theApp->Argv(j), "-s") == 0 ||
            strcmp(theApp->Argv(j), "--same") == 0 ||
            strcmp(theApp->Argv(j), "-sn") == 0 ||
            strcmp(theApp->Argv(j), "--samename") == 0 ||
            strcmp(theApp->Argv(j), "-cc") == 0 ||
            strcmp(theApp->Argv(j), "--contcol") == 0 ||
            strcmp(theApp->Argv(j), "-bc") == 0 ||
            strcmp(theApp->Argv(j), "--bestcol") == 0 ||
            strcmp(theApp->Argv(j), "-rc") == 0 ||
            strcmp(theApp->Argv(j), "--refcol") == 0 ||
            strcmp(theApp->Argv(j), "-o") == 0 ||
            strcmp(theApp->Argv(j), "--out") == 0) {

          i=j-1;
          break;
        }

        FInAsimov[name].push_back(theApp->Argv(j));
        nFiles[name]++;

      }


    } else if ((arg == "-o") || (arg == "--out")) {

      FOut = theApp->Argv(++i);

    } else if ((arg == "-s") || (arg == "--same")) {

      PlotOnSameCanvas = true;

    } else if ((arg == "-sn") || (arg == "--samename")) {

      NameSameCanvas = theApp->Argv(++i);

    } else if ((arg == "-cc") || (arg == "--contcol")) {

      ContoursColor = atoi(theApp->Argv(++i));

    } else if ((arg == "-bc") || (arg == "--bestcol")) {

      BestFitColor = atoi(theApp->Argv(++i));

    } else if ((arg == "-rc") || (arg == "--refcol")) {

      AsimovAColor = atoi(theApp->Argv(++i));

    } else {

      if (i + 1 > theApp->Argc() && nFiles.empty()) {
        std::cout << "NO SOURCES PROVIDED !" << std::endl;
        ShowUsage(theApp->Argv(0));
        exit(EXIT_FAILURE);
      }

    }

  }

  if (nFiles.empty()) {
    std::cout << "NO SOURCES DETECTED !" << std::endl;
    ShowUsage(theApp->Argv(0));
    exit(0);
  }

}

BiasParser::BiasParser(TApplication *App) {

  theApp = App;
  cAsimov.clear();
  cFDS.clear();

}

BiasParser::~BiasParser() {

  delete theApp;

}

void BiasParser::LaunchApp() {

  BiasParser::ProcessArgs();

}

void BiasParser::ShowUsage(std::string name) {

  std::cerr << "Usage: " << std::endl
            << "\t" << name << " -ia (--input-asimov) [FILE] -if (--input-fds) [FILE] " << std::endl
            << std::left << std::setw(30) << "\t-ia (--input-asimov)"
            << std::left << std::setw(50)
            << "Path to input asimov file" << std::endl
            << std::left << std::setw(30) << "\t-if (--input-fds)"
            << std::left << std::setw(50)
            << "Path to input FDS file" << std::endl
            << std::endl;

}

void BiasParser::ProcessArgs() {

  // Reading user input parameters
  if (theApp->Argc() < 2) {
    BiasParser::ShowUsage(theApp->Argv(0));
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < theApp->Argc(); i++) {

    std::string arg = theApp->Argv(i);

    if ((arg == "-h") || (arg == "--help")) {

      BiasParser::ShowUsage(theApp->Argv(0));
      exit(EXIT_FAILURE);

    } else if ((arg == "-ia") || (arg == "--input-asimov")) {

      cAsimov = theApp->Argv(++i);

    } else if ((arg == "-if") || (arg == "--input-fds")) {

      cFDS = theApp->Argv(++i);

    }

  } // END For arguments

}
