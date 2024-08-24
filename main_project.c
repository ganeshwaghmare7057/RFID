/** compiler directives **/
#include<reg52.h>
#include<string.h>

/** user defined header files **/
#include"delay.h"
#include"lcd.h"
#include"uart.h"
#include"i2c.h"
#include"keypad.h"

extern unsigned char rec_buff[25],k;
extern bit r_flag;

sbit fan_s0=P1^0;
sbit fan_s1=P1^1;

/*external interrupt routine*/
void ext_int(void) interrupt 0
{
	unsigned char pass[20],i;

	memset(pass,'\0',20);
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("Edit password:");
	delay_ms(1000);
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("password typing :");
	cmd_lcd(0xc0);
	/*for(i=0;i<4;i++)
	{
		//while(IsKeyPress());
		
		pass[i]=KeyValue();
		i2c_device_write(0xa0,0x00|i,pass[i]);
		putc_lcd(pass[i]);
		
	}*/
	
	i=0;
	while(1)
	{
		pass[i]=KeyValue();
			
			if(pass[i]=='#' && i!=0)
			{
				i2c_device_write(0xa0,0x00|i,pass[i]);
				pass[i]=0;
				break;
			}

			 if(pass[i]=='c' && (i!=0))
			{
				/*clear one character on lcd*/
				cmd_lcd(0x10);
				putc_lcd(' ');
				cmd_lcd(0x10);
				i--;
				continue;
			}

				/*display number and star immediately on lcd*/
				i2c_device_write(0xa0,0x00|i,pass[i]);
				putc_lcd(pass[i]);
				delay_ms(150);
				cmd_lcd(0x10);
				putc_lcd('*');
				i++;
	}
	
	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("password updated :");
	delay_ms(1000);

	cmd_lcd(0x01);
	cmd_lcd(0x80);
	str_lcd("waiting for card");
}



/** main function **/
int main()
{
	char card_no[]="12530066",i,ch[20],flag=0,tries;

	delay_ms(1000);
	uart_init();
	init_lcd();

	while(1)
	{
		tries=3;
			/*loop for card scanning*/
	  while(1)
	  {
		//uart_string("\r\nEnter data:\r\n");
		cmd_lcd(0x01);
		cmd_lcd(0x80);
		str_lcd("Waiting for card");
		r_flag=0;k=0;
		memset(rec_buff,'\0',25);
		while(r_flag==0);
		cmd_lcd(0x01);	
		cmd_lcd(0x80);
		str_lcd("card scanned");
		delay_ms(1000);
		cmd_lcd(0x01);
		cmd_lcd(0x80);

		if(strcmp(rec_buff,card_no)!=0)
		{
			str_lcd("invalid card");
			cmd_lcd(0xc0);
			str_lcd("retry scanning");
			delay_ms(2000);
			continue;			
		}

		else
		{
			str_lcd("card found..");
			cmd_lcd(0xc0);
			str_lcd(rec_buff);
			delay_ms(1000);
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			break;
		}
	}

	/*loop for typing password*/		

	while(1)
	{
	again:str_lcd("enter password");
		cmd_lcd(0xc0);
		memset(ch,'\0',20);
		/*for(i=0;i<4;i++)
		{
			ch=KeyValue();
			if(ch!= i2c_device_read(0xa0,0x00|i))
			{
				flag=1;
			}
		}*/
		
		i=0;
		while(1)
		{	
			ch[i]=KeyValue();
			
			if(ch[i]=='#' && i!=0)
			{
				if(ch[i]!= i2c_device_read(0xa0,0x00|i))
					flag++;

				break;
			}
			else if((ch[i]=='#' && i==0))
			{
				cmd_lcd(0x10);
				cmd_lcd(0x80);
				str_lcd("nothing entered");	
				cmd_lcd(0xc0);
				str_lcd("enter something");
				delay_ms(2000);
				cmd_lcd(0x01);
				cmd_lcd(0x80);
				goto again;		
			}

			 if(ch[i]=='c' && (i!=0))
		   	{
				/*clear one character on lcd*/
				cmd_lcd(0x10);
				putc_lcd(' ');
				cmd_lcd(0x10);
				i--;
				
				if(ch[i]!= i2c_device_read(0xa0,0x00|i))
					flag--;

				continue;
			}
			else if((ch[i]=='c' && i==0))
				continue;
			

			putc_lcd(ch[i]);
			delay_ms(100);
			cmd_lcd(0x10);
			putc_lcd('*');

			if(ch[i]!= i2c_device_read(0xa0,0x00|i))
				flag++;

			i++;
		}
		

		if(flag>0)
		{
			tries--;
			if(tries==0)
			{
				cmd_lcd(0x01);
				cmd_lcd(0x80);
				str_lcd("card blocked");
				delay_ms(1000);
				break;
			}

			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("invalid password");
			cmd_lcd(0xc0);
			str_lcd("enter again..");
			delay_ms(1000);
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			flag=0;
		}

		else
		{
			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("Door opened");
			/*write condition to open door*/
			fan_s0=1;
			fan_s1=0;
			delay_ms(50);
			fan_s0=0;
			
			delay_ms(3000);

			cmd_lcd(0x01);
			cmd_lcd(0x80);
			str_lcd("closing door");
			/*write condition to open door*/
			fan_s0=0;
			fan_s1=1;
			delay_ms(50);
			
			fan_s1=0;
			delay_ms(3000);
			break;
	  }		
	}	
  }
}//main end


