#include <stdio.h>
#include <math.h>
#include "MPU6050.h"

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (20)
#define q30  1073741824.0f
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
float fAX = 0.0f, fAY = 0.0f, fAZ = 0.0f;
short ax, ay, az, gx, gy, gz;

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from thei
 * driver(s).
 * TODO: The following matrices refer to the configuration on an internal test
 * board at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};

enum packet_type_e {
    PACKET_TYPE_ACCEL,
    PACKET_TYPE_GYRO,
    PACKET_TYPE_QUAT,
    PACKET_TYPE_TAP,
    PACKET_TYPE_ANDROID_ORIENT,
    PACKET_TYPE_PEDO,
    PACKET_TYPE_MISC
};

/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

static  unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

/* Handle sensor on/off combinations. */

static void run_self_test(void)
{
    int result;
//    char test_packet[4] = {0};
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
		printf("setting bias succesfully ......\n");
    }
	else
	{
		printf("bias has not been modified ......\n");
	}

    /* Report results. */
}

int MPU_init(void)
{
	if (mpu_init() != 0)
		return 0;
	printf("mpu initialization complete......\n");
	//mpu_set_sensor
	if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
		printf("mpu_set_sensor complete ......\n");
	else
	{
		printf("mpu_set_sensor come across error ......\n");
		return 0;
	}
	//mpu_configure_fifo
	if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
	{
		printf("mpu_configure_fifo complete ......\n");
	}
	else
	{
		printf("mpu_configure_fifo come across error ......\n");
		return 0;
	}
	//mpu_set_sample_rate
	if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))
	{
		printf("mpu_set_sample_rate complete ......\n");
	}
	else
	{
		printf("mpu_set_sample_rate error ......\n");
		return 0;
	}
	//dmp_load_motion_driver_firmvare
	if(!dmp_load_motion_driver_firmware())
	{
		printf("dmp_load_motion_driver_firmware complete ......\n");
	}
	else
	{
		printf("dmp_load_motion_driver_firmware come across error ......\n");
		return 0;
	}
	//dmp_set_orientation
	if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
	{
		printf("dmp_set_orientation complete ......\n");
	}
	else
	{
		printf("dmp_set_orientation come across error ......\n");
		return 0;
	}
	//dmp_enable_feature
	if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
				DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
				DMP_FEATURE_GYRO_CAL))
	{
		printf("dmp_enable_feature complete ......\n");
	}
	else
	{
		printf("dmp_enable_feature come across error ......\n");
		return 0;
	}
	//dmp_set_fifo_rate
	if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
	{
		printf("dmp_set_fifo_rate complete ......\n");
	}
	else
	{
		printf("dmp_set_fifo_rate come across error ......\n");
		return 0;
	}
	run_self_test();
	if(!mpu_set_dmp_state(1))
	{
		printf("mpu_set_dmp_state complete ......\n");
	}
	else
	{
		printf("mpu_set_dmp_state come across error ......\n");
		return 0;
	}
	
	return 1;
}

void MPU_getdata(void)
{
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4];

	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);	 
	if (sensors & INV_WXYZ_QUAT )
	{
		// 四元数融合算法，计算三个姿态角，其中fAZ偏航角与之前的参考示例计算的angz夹角不同
	 	 q0 = quat[0] / q30;
		 q1 = quat[1] / q30;
		 q2 = quat[2] / q30;
		 q3 = quat[3] / q30;
		 fAX = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
		 fAY = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
		 fAZ = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
	 }
	 if(sensors & INV_XYZ_GYRO)
	 {
		 gx = gyro[0];
		 gy = gyro[1];
		 gz = gyro[2];
	 }
	 if(sensors & INV_XYZ_ACCEL)
	 {
		 ax = accel[0];
		 ay = accel[1];
		 az = accel[2];
	 }
}

void MPU6050_ReturnTemp(float*Temperature)
{
//  short temp3;
//  uint8_t buf[2];

//  MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2); //读取温度值
//  temp3= (buf[0] << 8) | buf[1];
//  *Temperature=((double) (temp3 /340.0))+36.53;
  short tmp;
  if (0 == mpu_get_temp_reg(&tmp, 0))
    *Temperature=((double) (tmp /340.0))+36.53;
} 
