//
// Created by zsoldos on 26/03/19.
//

#ifndef ANALYSIS_UTILS_H
#define ANALYSIS_UTILS_H

#include <TColor.h>

enum ColorName{
  LITTLE_BOY_BLUE=0, // Nice BLUE
  CHERRY_TOMATO=1,   // Nice RED
  ARCADIA=2 ,        // Nice TEAL
  PINK_LAVENDER =3,  // Nice PINK
  BLOOMING_DAHLIA=4, // Nice BROWN
  ULTRA_VIOLET=5     // Nice VIOLET
};

inline std::vector<int> createColor(TColor *Color){

  std::vector<int> palette;

  // LITTLE BOY BLUE #6f9fd8
  palette.push_back(Color->GetColor("#6f9fd8"));
  // CHERRY TOMATO #e94b3c
  palette.push_back(Color->GetColor("#e94b3c"));
  // ARCADIA #00a591
  palette.push_back(Color->GetColor("#00a591"));
  // PINK LAVENDER #dbb1cd
  palette.push_back(Color->GetColor("#dbb1cd"));
  // BLOOMING DAHLIA #ec9787
  palette.push_back(Color->GetColor("#ec9787"));
  // ULTRA VIOLET #6b5b95
  palette.push_back(Color->GetColor("#ec9787"));

  return palette;

}

#endif //ANALYSIS_UTILS_H
