#include<reg52.h>

#include"delay.h"

sbit scl=P1^2;
sbit sda=P1^3;

void i2c_start()
{
	scl=0;
	sda=1;
	scl=1;
	sda=0;
}


void i2c_stop()
{
	scl=0;
	sda=0;
	scl=1;
	sda=1;
}


void i2c_ack()
{
	scl=0;
	sda=1;
	scl=1;
	while(sda);
}


void i2c_noack()
{
	scl=0;
	sda=1;
	scl=1;
}


void i2c_write(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		scl=0;
		sda=(dat&(0x80>>i))?1:0;
		scl=1;
	}

}


unsigned char i2c_read()
{
	unsigned char buff=0,i;
	for(i=0;i<8;i++)
	{		
		scl=0;
		scl=1;	
		if(sda)
			buff|=(0x80>>i);
	}
	return buff;
}

void i2c_device_write(unsigned char sa,unsigned char add,unsigned char dat)
{
	i2c_start();
	i2c_write(sa);
	i2c_ack();
	i2c_write(add);
	i2c_ack();
	i2c_write(dat);
	i2c_ack();
	i2c_stop();
	delay_ms(10);
}



unsigned char i2c_device_read(unsigned char sa,unsigned char add)
{
	unsigned char buff=0;
	i2c_start();
	i2c_write(sa);
	i2c_ack();
	i2c_write(add);
	i2c_ack();
	i2c_start();
	i2c_write(sa|1);
	i2c_ack();
	buff=i2c_read();
	i2c_noack();
	i2c_stop();
	return buff;
}





