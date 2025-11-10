import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# --- 1. 定义您想要拟合的函数模型 ---
# 选择一个负幂律模型: y = a * x^b + c
def power_law(x, a, b, c):
    """
    幂律函数模型
    :param x: 自变量
    :param a: 幅度参数
    :param b: 幂指数参数 (对于单调递减，b通常为负)
    :param c: 偏移量参数
    :return: y 值
    """
    return a * (x ** b) + c

# --- 2. 硬编码的原始数据 ---
x_original = np.array([2.2, 2.39, 2.59, 2.82, 3.06, 3.33, 3.36, 3.95, 4.30, 4.69, 5.13, 5.60, 6.13, 6.72, 7.37, 8.10, 8.91, 9.82, 10.83, 11.96, 20.05, 34.45, 60.12, 106.21, 191.77, 363.71, 753.62, 1757.95])
y_original = np.array([298, 296,  294,  292,  290,  288,  286,  284,  282,  280,  278,  276,  274,  272,  270,  268,  266,  264,  262,   260,   250,   240,   230,   220,    210,    200,    190,    180])

# --- 3. 执行曲线拟合 ---
# 初始猜测需要更仔细地设置
# 对于 y = a * x^b + c:
# 假设 b 是一个负数 (例如 -0.1 到 -0.5)
# a 可以通过代入一个点估算，例如用 (x[0], y[0]) 和 (x[-1], y[-1])
# y[0] = a * x[0]^b + c
# y[-1] = a * x[-1]^b + c
# 估算 c 的一个可能值 (例如 y[-1])，然后估算 a
initial_guess = [100.0, -0.1, 200.0] # 初始猜测: a=100, b=-0.1, c=200

try:
    popt, pcov = curve_fit(power_law, x_original, y_original, p0=initial_guess)
except RuntimeError as e:
    print(f"拟合失败: {e}")
    print("可能需要调整初始猜测 (p0)。")
    # 尝试另一个初始猜测
    initial_guess = [50.0, -0.2, 180.0]
    try:
        popt, pcov = curve_fit(power_law, x_original, y_original, p0=initial_guess)
    except RuntimeError as e2:
        print(f"第二次拟合也失败: {e2}")
        print("幂律模型可能不适合，或者需要更复杂的初始猜测策略。")
        exit()

# 获取拟合参数
a_fit, b_fit, c_fit = popt

# --- 4. 打印拟合得到的函数公式 ---
print("--- 拟合得到的函数公式 ---")
print(f"y = {a_fit:.6f} * x^({b_fit:.6f}) + {c_fit:.6f}")

# --- 5. 计算原始数据点的预测值 ---
y_predicted_at_original = power_law(x_original, a_fit, b_fit, c_fit)

# --- 6. 打印原始数据和预测值，以及残差 ---
print("\n原始数据与拟合函数预测值 (及其残差):")
print(f"{'x':<8} {'y_original':<12} {'y_predicted':<12} {'Residual':<12}")
print("-" * 50)
for x, y_orig, y_pred in zip(x_original, y_original, y_predicted_at_original):
    residual = y_orig - y_pred
    print(f"{x:<8.2f} {y_orig:<12.2f} {y_pred:<12.6f} {residual:<12.6f}")

# --- 7. 计算并打印拟合优度 R^2 ---
ss_res = np.sum((y_original - y_predicted_at_original) ** 2)
ss_tot = np.sum((y_original - np.mean(y_original)) ** 2)
r_squared = 1 - (ss_res / ss_tot)
print(f"\n拟合优度 R^2: {r_squared:.6f}")
print("\n完成。拟合公式为: y = {:.6f} * x^({:.6f}) + {:.6f}".format(a_fit, b_fit, c_fit))

# --- 8. 绘制图形 ---
x_smooth = np.linspace(x_original.min(), x_original.max(), 500)
y_smooth = power_law(x_smooth, a_fit, b_fit, c_fit)

plt.figure(figsize=(10, 6))
plt.scatter(x_original, y_original, color='red', label='Original Data Points', zorder=5, s=60)
plt.plot(x_smooth, y_smooth, color='blue', label=f'Fitted Curve: y = {a_fit:.3f} * x^({b_fit:.3f}) + {c_fit:.3f}', linewidth=2)
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Single Function Fitting (Power Law)')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.tight_layout()
plt.show()