#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "STM32_I2C.h"

int MPU_init(void);								// 初始化MPU6050模块，返回0表示失败
void MPU_getdata(void);						// 获取六轴数据和姿态角，更新全局变量
void MPU6050_ReturnTemp(float*Temperature);	// 读取MPU6050器件温度

extern float q0, q1, q2, q3;			// 四元数
extern float fAX, fAY, fAZ;				// 三个姿态角（pitch俯仰角, roll滚转角, yaw偏航角）
extern short ax, ay, az, gx, gy, gz;	// 六轴数据
