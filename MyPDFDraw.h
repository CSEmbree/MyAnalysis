/*
 * Title:    MyPDFDraw
 * Author:   Cameron S. Embree
 * Contact:  CSEmbree@gmail.com
 * Created:  10-Jun-2013
 * Edited:   10-Jun-2013
 * Notes:    
 */

#ifndef __MY_PDF_DRAW_H
#define __MY_PDF_DRAW_H


#include <stdlib.h> // exit()
#include <sstream>  // needed for internal io
#include <iomanip>
#include <vector>
#include <string>
#include <ostream>

using namespace std;

#define DEFAULT -1




class MyPDFDraw {
  
 public:
  //VARIABLES
  
  //METHODS
  MyPDFDraw(string _steeringFile);
  virtual ~MyPDFDraw(); //destructor
  
  void Print();
  void ReadSteering(const string _fileName);
  
        
  //ACCESSOR METHODS
  //bool IsDebugOn() const{return debug;};
  //string GetSteeringFilePath() const{return steeringFilePath;};
  //string GetSteeringFileDir() const{return steeringFileDir;};
  //string GetSteeringFileName() const{return steeringFileName;};

  //MUTATOR METHODS
  //void SetDebug(bool _debug);


 private:
  //VARIABLES
  bool debug;
  
  std::vector<std::string > pdfNameList;
  std::vector<std::string > pdfFlavorList;
  std::vector<double > Q2List;
  std::vector<double > nbinsList;
  std::vector<double > xminList;
  std::vector<double > xmaxList;

  string steeringFileName;    //name of steering file
        
  //METHODS
  bool FileExists(const string _fileName);
  //void SetVariablesDefault();
  //void SetSteeringFileNameAndDir(const string _path);
  //string GetEnv( const string & var);

};


//const string defaultPDFSetPath="PDFsets";


#endif
