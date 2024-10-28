#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#include "driver/i2c_master.h"

#define I2C_MASTER_SCL_IO 19     
#define I2C_MASTER_SDA_IO 18 
#define I2C_MASTER_NUM 0

void i2c_master_init(void);
#endif