
#include "root.h"
#include <vector>
//#include "utils.h"

#include "MyFrame.h"
#include "MyFrameData.h"
#include "MyData.h"
#include "MyCrossSection.h"

#include "AtlasStyle.h"

//void getPDF(const double& x, const double& Q, double* xf) {
//    evolvepdf_(&x, &Q, xf);
//}


int main(int argc, char** argv) 
{ 


  SetAtlasStyle();

  bool debug=false;
  //string inputname="atlas2012_top.txt";
  string inputname="atlascms_top.txt";
  if ( argc>1 ) inputname = string(argv[1]);
  std::cout << " reading steering file " << inputname << std::endl; 
//
//
//  char *fname= (char*) inputname.c_str();
  
  string test_name = "test_it.txt";

  TApplication *theApp = new TApplication("My ROOT Application",0,0);
  theApp->SetReturnFromRun(true);

  string psfile=inputname; psfile+=".eps";
  string psfile2;

  char *fname= (char*) inputname.c_str();
  MyCrossSection *mycross2= new MyCrossSection(fname);
  mycross2->Print();
  int nframe=mycross2->GetFrameNumber();
  cout<<"plot: Number of frames= "<<nframe<<endl;
   
  for (int iframe=0; iframe<nframe; iframe++){
   std::cout <<"plot: DrawinFrame for " << iframe << std::endl;
   mycross2->DrawinFrame(iframe);
   std::cout <<"plot: After DrawinFrame for iframe= " << iframe << std::endl;
   /*
   if (iframe==0) psfile2=psfile+"(";
   else           psfile2=psfile;
   if (iframe==nframe-1) psfile2=psfile+")";
   //std::cout << "Print mycross2" << std::endl;
   */
   //mycross2->GetMyFrame(iframe)->GetCanvas()->Print(psfile2.c_str());
   //std::cout << "Printed" << std::endl;

  }
  std::cout << "plot: Done with frame loop " << std::endl;

  theApp->Run(kTRUE);

 return 0;
}

