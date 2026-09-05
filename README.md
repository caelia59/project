# project
## Hardware
- STM32F103C8T6
- STM32 小车扩展板
- L298N 电机驱动模块
- 两个直流减速电机
- RPR220 循迹模块
- 7 位数码管
- 18650 电池及电源模块
## Progress
### Task 1
创建 GitHub 仓库并使用 Git 管理项目。
### Task 2
控制扩展板上的数码管，从 0 开始每秒加 1，到 9 后重新回到 0。
### Task 3
使用 L298N 驱动两个直流电机，并通过扩展板按键切换电机旋转方向。
### Task 4
读取循迹传感器状态，观察传感器在黑色和白色表面上的输出变化，并设计基础循迹逻辑。
### Task 5
完成小车组装。
![Car](documents/Car_Is_Ready.png)
### Task 6
实现按键控制小车启动和停止。
### Task 7
实现基础黑线循迹控制。
### Task 8
在基础循迹上增加弯道和丢线后的方向恢复逻辑，用于尝试 S 形路线。
### Task 9
加入 PID 控制，根据循迹误差调整左右电机速度，减少小车循迹过程中的抖动。
## Code
主要代码位于 `code` 文件夹中：
- `task2.c`
- `task3.c`
- `task4.c`
- `task6.c`
- `task7.c`
- `task8.c`
- `task9.c`

## Verified firmware (2026-09-05 update)
All tasks below were compiled and flashed onto the STM32-A90 car and verified on hardware:
- `code/firmware/task2.bin` - Task 2 seven-segment display
- `code/firmware/task3.bin` - Task 3 L298N motor reversal
- `code/firmware/task4.bin` - Task 4 RPR220 sensor test
- `code/firmware/task6.bin` - Task 6 button start/stop
- `code/firmware/line_follow_final.bin` - Tasks 7/8/9 final line-following firmware (PID + lost-line recovery)

Tasks 7, 8 and 9 share the same final firmware (`task7_project`, `task8_project`, `task9_project`):
- Task 7: basic line following with differential steering
- Task 8: S-curve following (lost-line recovery pivots back to the track)
- Task 9: PID control (Kp=120, Kd=2.0) to reduce wobble and follow the line smoothly
