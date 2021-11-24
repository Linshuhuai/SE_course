# assignment5 problem4 Least Squares Problems and QR
from math import pi
import numpy as np
import scipy as sp
from scipy import linalg
import matplotlib.pyplot as plt
from prettytable import PrettyTable


def is_pos_def(a):
    return np.all(np.linalg.eigvals(a) > 0)


# 产生样本数组
def example_x(n):
    return np.arange(0, 1 + 1e-10, 1 / (n - 1))


# 研究函数
def f(x: np.double) -> np.double:
    return 1.0 / (1.0 + np.square(x))


# 范德蒙矩阵
def vander(m, n, x):
    v = np.zeros((m, n))
    for i in range(m):
        for j in range(n):
            v[i][j] = x[i] ** j
    return v


# 有限傅里叶级数
def fourier(m, n, x):
    F = np.zeros((m, n))
    for i in range(m):
        for j in range(n):
            if 1 <= j <= n // 2:
                F[i][j - 1] = np.sin(j * pi * x[i])
            else:
                F[i][j - 1] = np.cos((j - n // 2) * pi * x[i])
    return F


# 3.1
M = N = [8, 16]
field_names = ("Matrix", "N", "Residual")
table = PrettyTable(field_names=field_names)
for n in N:
    # 范德蒙矩阵
    x = example_x(n)
    b = f(x)
    V = vander(n, n, x)
    P, L, U = sp.linalg.lu(V)
    LU_x_V = np.linalg.solve(U, np.linalg.solve(P.dot(L), b))  # LU分解后间接求解
    x_V_residual = np.linalg.norm(np.dot(V, LU_x_V.T) - b.T, 2)  # 求残差

    # 有限傅里叶级数
    F = fourier(n, n, x)
    P, L, U = sp.linalg.lu(F)
    LU_x_F = np.linalg.solve(U, np.linalg.solve(P.dot(L), b))  # LU分解后间接求解
    x_F_residual = np.linalg.norm(np.dot(F, LU_x_F.T) - b.T, 2)  # 求残差
    table.add_row(["Vander", str(n), str(x_V_residual)])
    table.add_row(["Fourier_Series", str(n), str(x_F_residual)])
print(table)


# 3.3
N = M = np.arange(4, 33, 2)
len = len(N)
field_names = ("N", 'isposdef(AV)', 'isposdef(AF)', 'cond(V)', 'cond(F)')
table = PrettyTable(field_names=field_names)
for i in range(0, len):
    x = example_x(M[i])
    V = vander(M[i], N[i], x)
    F = fourier(M[i], N[i], x)
    print(F)
    AV = np.dot(V.T, V)
    AF = np.dot(F.T, F)
    table.add_row([str(N[i]), str(is_pos_def(AV)), str(is_pos_def(AF)), str(np.linalg.cond(V)), str(np.linalg.cond(F))])
print(table)
