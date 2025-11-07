import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# 定义拟合函数
def exp_func(x, a, b, c):
    return a * np.exp(b * x) + c

# 数据
x_data = np.array([180, 200, 220, 240, 260, 280, 300])
y_data = np.array([2000, 400, 100, 40, 12, 6, 2])

# 👉 关键修改：给 b 一个负的初始值！
p0 = [2000, -0.05, 0]  # a≈起始值，b≈负值（衰减），c≈0或小值

# 拟合
popt, pcov = curve_fit(exp_func, x_data, y_data, p0=p0)

a, b, c = popt
print(f"拟合参数: a = {a:.4f}, b = {b:.6f}, c = {c:.4f}")
print(f"拟合公式: y = {a:.4f} * exp({b:.6f} * x) + {c:.4f}")

# 绘图
x_fit = np.linspace(170, 310, 200)
y_fit = exp_func(x_fit, a, b, c)

plt.figure(figsize=(8,5))
plt.scatter(x_data, y_data, color='red', label='原始数据', s=50)
plt.plot(x_fit, y_fit, 'b-', label=f'拟合曲线: y={a:.2f}·e^({b:.4f}x)+{c:.2f}', linewidth=2)
plt.xlabel('x')
plt.ylabel('y')
plt.title('指数衰减拟合')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()