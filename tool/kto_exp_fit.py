import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# 原始数据（你可以随意替换）
x_data = np.array([200, 220, 230, 240, 250, 260, 270, 280, 290, 300])
y_data = np.array([400, 120, 65, 40, 20, 12, 8, 5, 3, 2])

# ✅ 自动计算 x_center（推荐用均值，也可用中位数）
x_center = np.mean(x_data)  # 或 np.median(x_data)

# ✅ 自动计算初始参数 p0
# a: 取 x_center 对应的 y 值（插值或最近点）
idx = np.argmin(np.abs(x_data - x_center))
a_init = y_data[idx]  # 初始 a ≈ 中心点附近的 y 值

# b: 用两端点估算衰减率（简单线性回归 ln(y) ~ x）
# 注意：先去掉常数项影响，粗略估计
y_log = np.log(y_data - np.min(y_data) + 1e-6)  # 避免 log(0)
coeffs = np.polyfit(x_data, y_log, 1)
b_init = coeffs[0]  # 斜率即为 b 的粗略估计

# c: 取最小值作为常数项初始值
c_init = np.min(y_data) * 0.5  # 或直接设为 0

p0 = [a_init, b_init, c_init]

print(f"Auto-selected x_center = {x_center:.2f}")
print(f"Auto-initial guess: a={a_init:.2f}, b={b_init:.4f}, c={c_init:.2f}")

# 新的拟合函数（使用平移后的 x）
def exp_func_shifted(x_shifted, a, b, c):
    return a * np.exp(b * x_shifted) + c

# 拟合
x_shifted = x_data - x_center
popt, pcov = curve_fit(exp_func_shifted, x_shifted, y_data, p0=p0, maxfev=10000)

a, b, c = popt
print(f"\nFinal fitted parameters:")
print(f"Final formula: y = {a:.4f} * exp({b:.6f} * (x - {x_center:.2f})) + {c:.4f}")

# 🔍 计算每个原始数据点的预测值
x_shifted_for_pred = x_data - x_center
y_pred = exp_func_shifted(x_shifted_for_pred, a, b, c)

print(f"\n📊 原始数据点 vs 拟合预测值：")
print(f"{'x':>5} | {'y_raw':>7} | {'y_pred':>7} | {'误差(%)':>8}")
print("-" * 35)
for i in range(len(x_data)):
    error_percent = abs((y_pred[i] - y_data[i]) / y_data[i]) * 100 if y_data[i] != 0 else 0
    print(f"{x_data[i]:>5} | {y_data[i]:>7.2f} | {y_pred[i]:>7.2f} | {error_percent:>7.2f}%")

# 绘图验证
x_fit = np.linspace(min(x_data)-20, max(x_data)+20, 200)
x_fit_shifted = x_fit - x_center
y_fit = exp_func_shifted(x_fit_shifted, a, b, c)

plt.figure(figsize=(8,5))
plt.scatter(x_data, y_data, color='red', label='raw data', s=50)
plt.plot(x_fit, y_fit, 'b-', label=f'fit: y={a:.2f}·e^({b:.4f}(x-{x_center:.0f}))+{c:.2f}', linewidth=2)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Auto-adaptive Exponential Decay Fitting')
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()