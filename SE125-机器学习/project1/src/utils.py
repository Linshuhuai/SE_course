import numpy as np


def read_n(filepath, normalize):
    matrix = np.loadtxt(filepath, delimiter=',', skiprows=1, unpack=True)
    if normalize:
        for i in range(0, matrix.shape[0]):
            matrix[i] = normalization(matrix[i])
    return matrix.transpose()


def read_z(filepath, standardize):
    matrix = np.loadtxt(filepath, delimiter=',', skiprows=1, unpack=True)
    if standardize:
        for i in range(0, matrix.shape[0]):
            matrix[i] = standardization(matrix[i])
    return matrix.transpose()


def read_s(filepath, standardize):
    matrix = np.loadtxt(filepath, delimiter=',', skiprows=1, unpack=True)
    if standardize:
        for i in range(0, matrix.shape[0]):
            matrix[i] = sigmoid(matrix[i])
    return matrix.transpose()


def normalization(data):
    _range = np.max(data) - np.min(data)
    if _range == 0:
        return data - np.min(data)
    return (data - np.min(data)) / _range


def standardization(data):
    mu = np.mean(data, axis=0)
    _std = np.std(data, axis=0)
    if _std == 0:
        return data - mu
    return (data - mu) / _std


def sigmoid(x):
    return 1 / (1 + np.exp(-x))
