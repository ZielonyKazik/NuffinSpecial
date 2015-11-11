import numpy as np
import matplotlib.pyplot as plt

from sklearn import datasets

iris =  datasets.load_iris()

fig = plt.figure()

x = 0.1
y = 0.1

for i in reversed(range(len(iris.feature_names))):
	y_data = iris.data[:,i]
	for j in range(len(iris.feature_names)):
		x_data = iris.data[:,j]
		subplot = fig.add_axes([x, y, 0.17, 0.17])
		if i != j:
			for k in set(iris.target):
				target = iris.target == k
				subplot.scatter(x_data[target], y_data[target], color='bgr'[k], label=iris.target_names[k], marker='.')
		else:
			width = (max(x_data) - min(x_data)+ 0.1)/10;
			tab = []
			hight = []
			for bar in range(10):
				new = min(x_data)+bar*width	
				tab.append(new)
				p = 0
				for d in x_data :
					if ((d >= new) and (d < new + width)):
						p = p + 1
				hight.append(p)
			
			rects1 = subplot.bar(tab, hight, width, color='blue')
		x = x + 0.23
		if(j == 0):
			subplot.set_ylabel(iris.feature_names[j])
		if(i == 3):
			subplot.set_xlabel(iris.feature_names[i])
	y = y + 0.21
	x = 0.1
subplot.legend(bbox_to_anchor= (0.4,1.6), ncol=3)
plt.show()