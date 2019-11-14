print ("##############-script-starts-###############")

init()

#if Project is not None:  
#	Project.runProcess("New Task")


#print Project.PreDesign.HPC.findProperty("mass").valueToVariant()
#myMassVar = 20.4
#Project.PreDesign.HPC.findProperty("mass").setValueFromVariant(myMassVar)
#print Project.PreDesign.HPC.findProperty("mass")

openProject("v2500_hydrogen")

pro = currentProject()

val = pro.PreDesign.HPC_FN_18000.findGtProperty("mass")

print (val.valueToVariant())

val.setValueFromVariant(90)

print (val.valueToVariant())

print ("##############-script-ends-###############")