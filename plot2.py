import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime
import time
import csv
import matplotlib.pyplot as plt
with open('testHV.csv', newline='') as f:
    reader = csv.reader(f)
    your_list = list(reader)

# Create some mock data
#t = np.arange(0.01, 10.0, 0.01)

#t = np.linspace(0,len(your_list[0]),len(your_list[0]))
#tValArray = np.linspace(1545730073,1545730873,8)

tValArray = your_list[0]

t=[]
for tval in tValArray:
	#print("TVal : "+tval)
	tvalInt=0
	if tval:
		tvalInt = int(tval)
	timestamp = datetime.fromtimestamp(tvalInt)
	t.append(timestamp.strftime("%H:%M:%S"))
	#t.append(time.strftime("%H:%M:%S", tval))
print("TVAL shape : "+str(len(t)))
del t[40:]
from matplotlib.backends.backend_pdf import PdfPages
pp = PdfPages('foo.pdf')
	
#for i in range(len(your_list)):
plt.xticks(rotation=90)


count = 0;
for i in range(1,len(your_list),3):
#for i in range(1,16,3):

	data1=[]
	data2=[]
	
	print("I  : "+str(i))
	title = your_list[i]
	dataA1 = your_list[i+1] #np.exp(t)
	dataA2 = your_list[i+2] #np.sin(2 * np.pi * t)
	#print(type(dataA1))
	#print(type(dataA1[0]))
	#print(type(float(dataA1[0])))
	
	for volVal in dataA1:
		if volVal:
			data1.append(float(volVal))

	for curVal in dataA2:
		if curVal:
			data2.append(float(curVal))


	'''
	for vIndex in range(len(dataA1)):
		print("Vindex : "+str(vIndex) )
		print("DataA1 : "+str(dataA1[vIndex]))
		f = float(dataA1[vIndex])
		data1.append(f)
	'''
	#data1 = [float(str(dataA1[yval1])) for yval1 in range(len(dataA1))]
	#data2 = [float(dataA2[yval2]) for yval2 in range(len(dataA2))]

	#print("length of DataA1 : "+str(len(dataA1)))

	'''
	for yval1 in range(len(dataA1)):
		val = dataA1[yval1]
		print(type(val))
		data1.append(float(val))

	for yval2 in range(len(dataA2)):
		data2.append(float(dataA2[yval2]))
	'''
	
	'''
	try:could not convert <class, str> to float in python
		list1=[float(x) for x in dataA1]
		list2=[float(x) for x in dataA2]
	except ValueError,e:
		print "error",e,"on line",i
	'''

	#data1 = dataA1
	#data2 = dataA2

	del data1[40:]
	del data2[40:]

	print("Data1 Shape : "+str(len(data1)))
	print("Data2 Shape : "+str(len(data2)))
	
	fig, ax1 = plt.subplots()
	plt.xticks(rotation=90)

	print("Title : "+title[0])
	plt.title(title[0])

	color = 'tab:red'
	ax1.set_xlabel('Time ("HH:MM:SS")')
	ax1.set_ylabel('Voltage (Volts)', color=color)
	ax1.plot(t, data1, color=color,marker="o")
	ax1.tick_params(axis='y', labelcolor=color)
	#ax1.set_ylim([700,1800])

	ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

	color = 'tab:blue'
	ax2.set_ylabel('Current (MicroAmps)', color=color)  # we already handled the x-label with ax1
	ax2.plot(t, data2, color=color,marker="+")
	ax2.tick_params(axis='y', labelcolor=color,)
	#ax2.set_ylim([100,800])
	
	fig.tight_layout()  # otherwise the right y-label is slightly clipped
	#plt.savefig("plots.pdf")
	plt.savefig(pp,format='pdf')
	#plt.savefig(pp,format='pdf')
	#plt.savefig(pp,format='pdf')
	#plt.savefig(pp,format='pdf')
	#i = i + 2
	#print("i  : "+str(i))
	
pp.close()
#pp.savefig(fig)

#plt.show()
