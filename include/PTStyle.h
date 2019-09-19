//
// Created by zsoldos on 02/12/18.
//

#ifndef ANALYSIS_PTSTYLE_H
#define ANALYSIS_PTSTYLE_H

#include <TStyle.h>
#include <TROOT.h>

class PTStyle {

 protected:
  TStyle *style;

  std::string styleName;
  std::string styleOptName;

 public:

  PTStyle();
  PTStyle(const std::string &styleName, const std::string &styleOptName = "Personal default style");
  ~PTStyle();

  void SetT2KStyleVariables();
  void SetCustomStyleVariables();

};

#endif //ANALYSIS_PTSTYLE_H
