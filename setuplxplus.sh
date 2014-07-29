#
#
### generic setup to be run at the start of EVERY LXPLUS session
export BASEDIR=/afs/cern.ch/user/c/cembree/public/progs
#
#
### home directory of installation like APPLGrid
export MYHOME=/afs/cern.ch/user/c/cembree/public
#
#
### setup for SLC6
export platform=x86_64-slc6-gcc46-opt
#
export external=/afs/cern.ch/sw/lcg/external
export releases=/afs/cern.ch/sw/lcg/app/releases
export contrib=/afs/cern.ch/sw/lcg/contrib
#
#export GCC_INSTALL_DIR=${contrib}/gcc/4.3.2/${platform}
source ${contrib}/gcc/4.6/${platform}/setup.sh
#
#
### Add program paths to standard paths $PATH & LDPATH
export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.05/${platform}/root
#export LHAPDFPATH=/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.8/${platform}
export LHAPDFPATH=${BASEDIR}/lhapdf
export FASTJET=/afs/cern.ch/sw/lcg/external/fastjet/3.0.3/${platform}
export HOPPET=${BASEDIR}/hoppet
export APPLGRID=${BASEDIR}/applgrid
export SHERPA=${BASEDIR}/sherpa
export PKG_CONFIG_PATH=/afs/cern.ch/user/c/cembree/public/progs/mcgrid/mcgrid-1.0.1/
#
#
#export YODA=/afs/cern.ch/user/c/cembree/public/progs/
#export FASTJET=/afs/cern.ch/user/c/cembree/public/progs/
#export BOOST=/afs/cern.ch/user/c/cembree/public/progs/
#export GSL=/afs/cern.ch/user/c/cembree/public/progs/
#
export BZR=/afs/cern.ch/sw/lcg/external/bzr/2.1.0_python2.5/x86_64-slc5-gcc44-opt/
export BZRLIB=/afs/cern.ch/sw/lcg/external/bzr/2.1.0_python2.5/x86_64-slc5-gcc44-opt/lib/python2.5/site-packages/
#
#
#explicit
export PATH=${FASTJET}/bin:${HOPPET}/bin:${APPLGRID}/bin:${SHERPA}/bin:${ROOTSYS}/bin:${LHAPDFPATH}/bin:${PATH}
export LD_LIBRARY_PATH=${FASTJET}/lib:${HOPPET}/lib:${APPLGRID}/lib:${SHERPA}/lib:${ROOTSYS}/lib:${LHAPDFPATH}/lib:${LD_LIBRARY_PATH}
#
#export PYTHONPATH=${BZRLIB}:${BZRLIB}/bzrlib:${PYTHONPATH}
#
#
#source ${BASEDIR}/rivetenv.sh #sets up rivet env variables
#export RIVET_ANALYSIS_PATH=${BASEDIR}/rivet/Rivet-2.1.2/plugins:${BASEDIR}/libs
#export RIVET_REF_PATH=${RIVET_ANALYSIS_PATH}:$RIVET_REF_PATH
#export RIVET_INFO_PATH=${RIVET_ANALYSIS_PATH}:$RIVET_INFO_PATH
#export RIVET_PLOT_PATH=${RIVET_ANALYSIS_PATH}:$RIVET_INFO_PATH
#
#
export  ARCH="-m64"
export CXXFLAGS=$ARCH
export F90FLAGS=$ARCH
export CFLAGS=$ARCH
export FFLAGS=$ARCH
export LDFLAGS=$ARCH
