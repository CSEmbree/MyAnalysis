
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string>
//#include "AtlasStyle.h"

#include "MyPDFDraw.h"


int main(int argc, char** argv) 
{
  std::string steeringFileName = "steering_mypdfdraw.txt";
  
  MyPDFDraw* pdfd = new MyPDFDraw(steeringFileName);//new MyPDFDraw::MyPDFDraw(steeringFileName);
  
}
