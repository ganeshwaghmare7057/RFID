#ifndef i2c_h
#define i2c_h

extern void start(void);
extern void stop(void);
extern void ack(void);
extern void noack(void);
extern void i2c_write(unsigned char );
extern unsigned char i2c_read(void);
extern void i2c_device_write(unsigned char ,unsigned char ,unsigned char );
extern unsigned char i2c_device_read(unsigned char ,unsigned char );

#endif


