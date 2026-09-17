#pragma once

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define MPU6050_REG_CHIP_ID 0x75
#define MPU6050_CHIP_ID     0x68
#define MPU6500_CHIP_ID     0x70
#define MPU9250_CHIP_ID     0x71
#define MPU6880_CHIP_ID     0x19

#define MPU6050_REG_SMPLRT_DIV 0x19

#define MPU6050_REG_GYRO_CFG  0x1B
#define MPU6050_GYRO_FS_SHIFT 3

#define MPU6050_REG_ACCEL_CFG  0x1C
#define MPU6050_ACCEL_FS_SHIFT 3

#define MPU6050_REG_DATA_START 0x3B

#define MPU6050_REG_PWR_MGMT1 0x6B
#define MPU6050_SLEEP_EN      (1U << 6)
