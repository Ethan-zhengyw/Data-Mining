import numpy as np
import pandas as pd
from regression import loadData


matInput, matExpect = loadData('train_temp.csv')
w = np.mat(np.loadtxt('weight.txt'))

res = abs(matInput * w.T - matExpect)

for i in range(res.shape[0]):
    res[i,0] *= res[i, 0]

aver = (res.sum() / res.shape[0]) ** 0.5
print aver
