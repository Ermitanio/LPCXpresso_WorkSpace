#include "chip.h"


void InitI2C(void);

/*
 * I2C_XFER_Config
 * 	Función de escritura y lectura generalizada.
 */
void I2C_XFER_config (I2C_XFER_T * xfer,uint8_t *rbuf, int rxSz, uint8_t slaveAddr, I2C_STATUS_T status, uint8_t * wbuf, int txSz);

/*
 * I2C_ReadByte
 * 		slaveAddr: Adress del dispositivo esclavo
 * 		wbuf: Registro a leer del esclavo (Buffer de escritura)
 * 		rbuf: Puntero a memoria de almacenamiento de la lectura (Buffer de lectura)
 */
void I2C_ReadByte(uint8_t slaveAddr, uint8_t wbuf, uint8_t *rbuf);

/*
 * I2C_WriteByte
 * 		slaveAddr: Adress del dispositivo esclavo
 * 		regAddr: Registro a escribir del esclavo (Buffer de escritura)
 * 		write_value: Valor de la escritura
 */
void I2C_WriteByte(uint8_t slaveAddr, uint8_t regAddr, uint8_t write_value);

/*
 * I2C_Read
 * 		slaveAddr: Adress del dispositivo esclavo
 * 		wbuf: Registro a leer del esclavo (Buffer de escritura)
 * 		rbuf: Puntero a memoria de almacenamiento de la lectura (Buffer de lectura)
 * 		cant: Cantidad de datos (corridos) a leer.
 */
void I2C_Read(uint8_t slaveAddr, uint8_t wbuf, uint8_t *rbuf ,uint8_t cant);

