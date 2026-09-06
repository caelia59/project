# RoboMaster 暑期项目

## 硬件

- STM32F103C8T6（STM32-A90 小车）
- L298N 电机驱动模块
- 两个直流减速电机
- RPR220 循迹模块（两个）
- 七段数码管
- 18650 电池

## 任务进度

### Task 1：建仓库
在 GitHub 创建本仓库，用 git 管理代码。

### Task 2：数码管计数
用 PA0-PA7 驱动七段数码管，从 0 开始每秒加 1，到 9 后回到 0，循环显示。
代码在 code/task2_project/。

### Task 3：电机控制
用 PB6-PB9 控制 L298N 驱动两个电机正转，按一下扩展板按键电机反转，可以反复切换。
代码在 code/task3_project/。

### Task 4：循迹传感器测试
读两个 RPR220 传感器（PB12/PB13），把两个传感器的状态组合成 0-3 显示在数码管上，观察传感器在黑色和白色上的输出变化。
代码在 code/task4_project/。

### Task 5：组装小车
装好小车后拍照，放在 documents/Car_Is_Ready.png。

### Task 6：按键启停
按一下按键小车开始前进，再按一下停止。
代码在 code/task6_project/。

### Task 7/8/9：循迹
- Task 7：双传感器循迹，小车沿黑线走
- Task 8：走 S 弯等复杂线路
- Task 9：加入 PID 控制，减小抖动，走得更稳

最终版代码在 code/task9_project/，task7.c、task8.c 是中间版本的参考代码。

## 代码目录

- code/task2_project/  数码管
- code/task3_project/  电机控制
- code/task4_project/  传感器测试
- code/task6_project/  按键启停
- code/task9_project/  循迹最终版（PID）
