init()

print "##############-----------###############"


#if Project is not None:  
#	Project.runProcess("New Task")


#print Project.PreDesign.HPC.findProperty("mass").valueToVariant()
#myMassVar = 20.4
#Project.PreDesign.HPC.findProperty("mass").setValueFromVariant(myMassVar)
#print Project.PreDesign.HPC.findProperty("mass")

proj = openProject("MeanlineTest")

val = proj.PreDesign.HPC.findProperty("mass")

print val.valueToVariant()

val.setValueFromVariant(5)

print val.valueToVariant()
