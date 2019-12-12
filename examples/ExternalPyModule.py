#import sys
#sys.path.append('D:\\devel\\Git\\GTlab_Python_Module\\examples')
#import ExternalPyModule

from GtCalculators import *
from GtLogging import gtDebug

def createDesignPointCalc():
	# <-- Off-Design Point Calculator -->
	offDesignPointCalculator = GtpOffDesignCalculator("Off-Design Point Calculator")
	
	eQS = offDesignPointCalculator.createGtpEquationSystem("EQS")
	eQS.controlsUUIDsSteady = ()
	eQS.designMode = False
	
	bYPASSDUCTPLCdesign = eQS.createGtpBoundaryCondition("BYPASSDUCT.PLCdesign")
	bYPASSDUCTPLCdesign.BCname = "BYPASSDUCT.PLCdesign"
	bYPASSDUCTPLCdesign.BCval = 0.0
	bYPASSDUCTPLCdesign.BCunit = "[-]"
	
	bYPASSDUCTQU = eQS.createGtpBoundaryCondition("BYPASSDUCT.QU")
	bYPASSDUCTQU.BCname = "BYPASSDUCT.QU"
	bYPASSDUCTQU.BCval = 0.0
	bYPASSDUCTQU.BCunit = "[W]"
	# <--------------------------------->
	
	gtDebug() << "Design Point Calculator created!"
	return offDesignPointCalculator