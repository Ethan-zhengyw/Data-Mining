# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd


def loadData(path):
    """
    读取文件中的数据，将数据分为两个矩阵
    
    :matInput: 除了最后一列外的所有列，且在最前面插入一列1
    :matOutput: 即数据的最后一列
    """

    # labels = ['Id'];
    # for i in range(384):
    #     labels.append('value' + str(i))
    # labels.append('reference')

    print 'loading data...'

    data = pd.read_csv(path)
    data = np.mat(data)

    matExpect = data[:, -1]

    matInput = []
    for i in range(data.shape[0]):

        matInput.append([1])
        for j in range(1, data[0].shape[1] - 1):
            matInput[i].append(data[i, j])

    print "Finished loadData."

    return np.mat(matInput), np.mat(matExpect)


def getMatError(matInput, matWeight, matExpect):
    """
    根据输入的已知变量和估计的权重计算结果，和期望值比较并求均方根误差的1/2
    """

    matDiff = getMatDiff(matInput, matWeight, matExpect)
    size = matDiff.shape[0]
    
    for i in range(size):
        matDiff[i] = matDiff[i] ** 2
    
    matError = matDiff.sum() / (2 * size)

    print "matError: %f" % matError
    return matError


def getMatDiff(matInput, matWeight, matExpect):
    """ 计算结果和期望的差值
    """
    return (matInput * matWeight - matExpect)


def gradDescent(matInput, matExpect, alpha):
    matWeight = np.ones((matInput.shape[1], 1))

    xNum = matInput.shape[0]
    bNum = matInput.shape[1]

    matError = getMatError(matInput, matWeight, matExpect)
    while matError > 0.1:
        # if matError < 700:
        #     alpha /= 5
        print 'Updating weight...'
        matDiff = matInput * matWeight - matExpect
        newB = np.zeros(matWeight.shape)
        for i in range(bNum):
            for j in range(xNum):
                newB[i, 0] += matDiff[j, 0] * matInput[j, i]
        newB /= xNum
        matWeight += alpha * newB

        print 'Calculating matError...'
        matError = getMatError(matInput, matWeight, matExpect)
        np.savetxt('weight.txt', matWeight)

    return matWeight


if __name__ == '__main__':
    matInput, matExpect = loadData('train_temp.csv')
    # print matExpect
    matWeight = gradDescent(matInput, matExpect, 0.075)
    # weight = np.loadtxt('weight2.txt')

    # my result
    print matWeight

    ## video result
    #matWeight = np.mat('18.763;6.2698;-16.203;-2.673;30.271')
    #print test*matWeight
