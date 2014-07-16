/*
 * Title:    MyGrid
 * Author:   T. Carli, C. Embree
 * Contact:  tancredi.carli@cern.ch
 * Created:  16-Jul-2013
 * Edited:   16-Jul-2014
 * Notes:    Handles communication to the grid and details about it's generation
 */

#ifndef MyGrid_H
#define MyGrid_H

using namespace std;

#include <iostream>
//#include <iomanip>
#include <string>


//root
#include "root.h"
//#include "TMatrixTLazy.h"
//#include "TMatrixT.h"
#include "appl_grid/appl_grid.h"


class MyGrid {
 private:

  bool debug;

  std::string steername;
  std::string gridpath;
  std::string gridname;
  std::string gridgeneratorid;
  std::string gridntupleid;
  bool normtot;
  bool dividedbybinwidth;
  
  double scaley;
  double scalex;
  
  std::string xunits;
  std::string yunits;
 
 public:

  MyGrid(string fname);

  void ReadSteering(string fname);
  bool ValidateSettings();
  void Print();
  void ScaleGraph(TGraphAsymmErrors *g1, double scalex, double scaley);
  void NormaliseGraph(TGraphAsymmErrors* g1, double yscale, double xscale, bool normtot, bool divbinwidth);
  void split_string(std::string str, std::vector<std::string>& split_results, std::string delimiters);
  bool file_exists(const string& s);

  string GetSteeringName() { return steername; };
  string GetGridPath() { return gridpath; };
  string GetGridName() { return gridname; };
  string GetGridGeneratorID() { return gridgeneratorid; };
  string GetGridNTupleID() { return gridntupleid; };
  bool GetNormTot() { return normtot; };
  bool GetDividedByBinWidth() { return dividedbybinwidth; };
  double GetScaleX() { return scalex; };
  double GetScaleY() { return scaley; };
  string GetXUnits() { return xunits; };
  string GetYUnits() { return yunits; };
  TH1D *GetReference();

};


#endif
