
CXX = g++
F77 = gfortran
DEBUG = -g

FFLAGS   += -O3 -fPIC 
CXXFLAGS += -O3 -fPIC -fpermissive -std=c++0x
#CXXFLAGS += -O3 -fPIC -fpermissive 
LDFLAGS +=  -fno-automatic -fno-f2c -O3  # -I./src/Inc -Iobj

# root
ROOTINCS = $(shell root-config --cflags) 
ROOTLIBS = $(shell root-config --glibs) 
ROOTARCH = $(findstring -m64, $(ROOTINCS) )

#LHAPDF
LHAPDFINCS = -I$(shell lhapdf-config --prefix)/include
LHAPDFDIR  = $(shell lhapdf-config --prefix)/lib
LHAPDFLIBS = -L$(LHAPDFDIR) -lLHAPDF

# applgrid
APPLCXXFLAGS = $(shell applgrid-config --cxxflags)
APPLCLIBS    = $(shell applgrid-config --ldcflags)
APPLFLIBS    = $(shell applgrid-config --ldflags)
#APPLCLIBS    = -L/afs/cern.ch/user/c/cembree/public/progs/lib -lAPPLgrid -m64

# hoppet
HOPPETLIBS =  $(shell hoppet-config --libs)


# get the fortran runtime library for linking fortran 

FRTLLIB = $(shell gfortran $(CXXFLAGS) -print-file-name=libgfortran.a)
FRTLIB  = -L$(subst /libgfortran.a, ,$(FRTLLIB) ) -lgfortran

HOPPETINCS=$(shell hoppet-config --cxxflags)
HOPPETLIBS=$(shell hoppet-config --libs)

# now set up the compile and link flags and libs
CXXFLAGS += $(ROOTARCH) $(ROOTINCS) $(APPLCXXFLAGS) $(LHAPDFINCS) $(HOPPETINCS) 

CLIBS += $(APPLCLIBS) $(HOPPETLIBS) $(ROOTLIBS) $(FRTLIB) $(LHAPDFLIBS)
FLIBS += $(LHAPDFLIBS) $(HOPPETLIBS) $(APPLFLIBS) $(ROOTLIBS) $(FRTLIB)

OFILE= MyFrame.o MyFrameData.o MyData.o MyPDF.o MyBand.o MyGrid.o MyCrossSection.o AtlasStyle.o lhapdf_string_interface.o 
#

install : all
all : plot testmypdf

plot: plot.o $(OFILE) 
	$(CXX) $(LDFLAGS) $(DEBUG) -o $@ $(OFILE) $<  $(CLIBS)

testmypdf: testmypdf.o $(OFILE)
	$(CXX) $(LDFLAGS) -o $@  $(OFILE) $< $(CLIBS) $(LHAPDFLIBS) 


.SUFFIXES : .cxx .o .f .c 

.f.o :
	$(F77) $(FFLAGS)   -c $<

.cxx.o:	 
	$(CXX) $(CXXFLAGS) -c $< 


clean:
	rm -rf ./.libs ./.obj *.lo *.o *.la  *~
