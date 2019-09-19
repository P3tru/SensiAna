# Basic Makefile

### Compilers
CC  = gcc
CXX = g++

DEBUG_LEVEL    = -g
EXTRA_CCFLAGS  = -W -Wall -std=c++11
CPPFLAGS       = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)
CCFLAGS        = $(CPPFLAGS)

RM = rm -f

SRCDIR := src
INCDIR := include

### ROOT
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs)

CPPFLAGS  += -I$(ROOTSYS)/include -I$(INCDIR) $(ROOTCFLAGS)
EXTRALIBS  = $(ROOTLIBS)

SRCS = $(wildcard $(SRCDIR)/*.cc)
OBJS = $(subst .cc,.o,$(SRCS))

.PHONY: all clean PlotNuMu
.DEFAULT_GOAL = Plotdm32VSt23

help:
	@grep -h -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'

all: Plotdm32VSt23 

Plotdm32VSt23: Plotdm32VSt23.o $(OBJS)
	$(CXX) $(CPPFLAGS) -o Plotdm32VSt23 Plotdm32VSt23.cc $(OBJS) $(EXTRALIBS)
	$(RM) $(OBJS)

ComputeBias: ComputeBias.o $(OBJS)
	$(CXX) $(CPPFLAGS) -o ComputeBias ComputeBias.cc $(OBJS) $(EXTRALIBS)
	$(RM) $(OBJS)

clean:
	$(RM) Plotdm32VSt23 ComputeBias $(OBJS)

# OLD with bad nue PDFs which yield to wrong MakeBreakDown
FILENUMU?= $(PTHETADIR)/outputs/heppc/IntAsimov_RFNueNumu-WithEb_Etheta_NUMU_SHAPE2_10k.root
FILENUMUBAR?= $(PTHETADIR)/outputs/heppc/IntAsimov_NuMuBar_ToyXP_Asimov_Template_Asimov.root
FILENUMU_EREC?= $(PTHETADIR)/outputs/heppc/IntAsimov_RFNueNumu-WithEb_Erec_NUMU_SHAPE1_10k.root
FILENUMUBAR_EREC?= $(PTHETADIR)/outputs/heppc/IntAsimov_RFNueNumu-WithEb_Erec_NUMUBAR_SHAPE1_10k.root

# # 20190604 with corrected nue PDFs
# FILENUMU?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_RFNumuEtheta_PDFNumuEtheta_NUMU_SHAPE2_10k_20190604.root
# FILENUMUBAR?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_RFNumuEtheta_PDFNumuEtheta_NUMUBAR_SHAPE2_10k_20190604.root
# FILENUMU_EREC?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_RFNumuErec_PDFNumuEtheta_NUMU_SHAPE1_10k_20190604.root
# FILENUMUBAR_EREC?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_RFNumuErec_PDFNumuEtheta_NUMUBAR_SHAPE1_10k_20190604.root

# 20190604 with corrected nue PDFs AND NO Eb !
FILENUMU_NOEB?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_NoEb_PDFNumuEtheta_NUMU_SHAPE2_10k_20190604.root
FILENUMUBAR_NOEB?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_NoEb_PDFNumuEtheta_NUMUBAR_SHAPE2_10k_20190604.root
FILENUMU_NOEB_EREC?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_NoEb_PDFNumuEtheta_NUMU_SHAPE1_10k_20190604.root
FILENUMUBAR_NOEB_EREC?= $(PTHETADIR)/outputs/heppc/IntAsimov_BANFF_Datafit_NoEb_PDFNumuEtheta_NUMUBAR_SHAPE1_10k_20190604.root


PlotNuMu: Plotdm32VSt23 ## Make NuMu contours Etheta sampling
	./Plotdm32VSt23 -n NuMuEtheta $(FILENUMU)
PlotNuMuBar: Plotdm32VSt23 ## Make NuMuBar contours Etheta sampling
	./Plotdm32VSt23 -n NuMuBarEtheta $(FILENUMUBAR)

PlotNuMuErecOnly: Plotdm32VSt23 ## Make NuMu contours Erec only sampling
	./Plotdm32VSt23 -n NuMuErec $(FILENUMU_EREC)
PlotNuMuBarErecOnly: Plotdm32VSt23 ## Make NuMuBar contours Erec only sampling
	./Plotdm32VSt23 -n NuMuBarErec $(FILENUMUBAR_EREC)

PlotNuMuCompEthetaVSErec: Plotdm32VSt23 ## Make NuMu contours comparing Erec and Etheta sampling
	./Plotdm32VSt23 -s -sn NuMu -n NuMuErecOnly $(FILENUMU_EREC) -n NuMuEtheta $(FILENUMU)
PlotNuMuBARCompEthetaVSErec: Plotdm32VSt23 ## Make NuMuBar contours comparing Erec and Etheta sampling
	./Plotdm32VSt23 -s -sn NuMuBar -n NuMuBarErecOnly $(FILENUMUBAR_EREC) -n NuMuBarEtheta $(FILENUMUBAR)

PlotNuMuCompEthetaWithEbVSwoEb: Plotdm32VSt23 ## Make NuMu Etheta contours comparing with/wo Eb
	./Plotdm32VSt23 -s -sn Etheta -n NuMuWithEb $(FILENUMU) -n NuMuWoEb $(FILENUMU_NOEB)
PlotNuMuBARCompEthetaWithEbVSwoEb: Plotdm32VSt23 ## Make NuMuBar Etheta contours comparing with/wo Eb
	./Plotdm32VSt23 -s -sn Etheta -n NuMuBarWithEb $(FILENUMUBAR) -n NuMuBarWoEb $(FILENUMUBAR_NOEB)

PlotNuMuCompErecWithEbVSwoEb: Plotdm32VSt23 ## Make NuMu Erec contours comparing with/wo Eb
	./Plotdm32VSt23 -s -sn Erec -n NuMuWithEb $(FILENUMU_EREC) -n NuMuWoEb $(FILENUMU_NOEB_EREC)
PlotNuMuBARCompErecWithEbVSwoEb: Plotdm32VSt23 ## Make NuMuBar Erec contours comparing with/wo Eb
	./Plotdm32VSt23 -s -sn Erec -n NuMuBarWithEb $(FILENUMUBAR_EREC) -n NuMuBarWoEb $(FILENUMUBAR_NOEB_EREC)

