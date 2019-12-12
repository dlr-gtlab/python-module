#For using this external module in a Python Task insert the following lines 
#to the GTlab Python Editor.
#import sys
#sys.path.append('D:\\devel\\Git\\GTlab_Python_Module\\examples')
#import ExternalPyModule

from GtCalculators import *
from GtLogging import gtDebug

def createCalc():
	# <-- Off-Design Point Calculator -->
	offDesignPointCalculator = GtpOffDesignCalculator("Off-Design Point Calculator")
	
	eQS = offDesignPointCalculator.createGtpEquationSystem("EQS")
	eQS.controlsUUIDsSteady = ()
	eQS.designMode = False
	
	bURNERETAdesign = eQS.createGtpBoundaryCondition("BURNER.ETAdesign")
	bURNERETAdesign.BCname = "BURNER.ETAdesign"
	bURNERETAdesign.BCval = 0.0
	bURNERETAdesign.BCunit = "[-]"
	# <--------------------------------->
	
	return offDesignPointCalculator