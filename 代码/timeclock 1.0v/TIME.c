#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int 


sbit LED_ON=P1^4;
sbit key_on=P3^2;
sbit key_hour=P3^4;
sbit key_minute=P3^6;
sbit key_second=P3^7;
sbit dula=P2^6;
sbit wela=P2^7;
sbit bee=P2^2;
sbit wanduan=P1^7;


uchar code dula_num[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x40};
uchar code wela_num[]={0xfe,0xfd,0xfb,0xf7,0xef,0Xdf,0xbf,0x7f};
uchar code days[]={31,28,31,30,31,30,31,31,30,31,30,31};

void h_m_s();
void h_m_s_second();
void date_display();
void display();
void c_display();
void ms_display();
void clock();
void key_scan();
void key_Function();

void LED_SEC();
void LED_TIME();
void LED_CLOCK();
void LED_TEM();


void delayus(uint t);
void ds_reset();
bit ds_read_bit();
uchar ds_read_byte();
void ds_write_byte(uchar dat);
uint read_temperature();
void tem_display(uint temp);
void temperature();

bit flag_leap=0,flag_starttime=0,flag_startsecond=0;
uchar hour=12,minute=30,second,c_hour,c_minute,c_second,ms_minute,ms_second;
uchar key_return,count,led_time1,led_clock1,led_sec1,led_tem1;
uchar ms_10,ms_3,ms_40,ms_50,ms_30;
uchar month=5,day=20;
uint ms_1000;
uint ms_sec;
uint m_second;
uint temp,year=2016;


void main()
{
		led_sec1=0xfe;
	  led_clock1=0x00;
	  led_time1=0xfe;
		led_tem1=0xaa;
		bee=1;
		TMOD=0x01;
		TH0=(65536-1000)/256;
		TL0=(65536-1000)%256;
		EA=1;
		ET0=1;
		
		TR0=1;
	LED_ON=0;
		while(1)
		{
			if(ms_10==10)
			{
				ms_10=0;
			  key_scan();
			}
			if(ms_3==3)
			{
				ms_3=0;
			  switch(count)
				{
					case 0: display();LED_TIME();break;
					case 1: display();break;
					case 2: c_display();break;
					case 3: ms_display();LED_SEC();break;
					case 4: date_display();break;
					case 5: tem_display(read_temperature());temperature();LED_TEM();break;
					default:break;
				}
			}
			clock();
			if((year%4==0&&year%400!=0)||year%100==0)
			   flag_leap=1;
			else
				flag_leap=0;
			
		}
}


void time_0() interrupt 1
{
		TH0=(65536-1000)/256;
		TL0=(65536-1000)%256;
		if(flag_starttime==0)
		{
		  ms_sec++;
		}
		if(flag_startsecond==1)
		{
		  m_second++;
		}
		ms_3++;
		ms_10++;
		ms_30++;
		ms_40++;
		ms_1000++;
		ms_50++;
		h_m_s();
		h_m_s_second();
}
void h_m_s()
{
	if(ms_sec==1000)
	{
		ms_sec=0;
		second++;
		if(second>59)
		{
			second=0;
			minute++;
			if(minute>59)
			{
				minute=0;
				hour++;
				if(hour>23)
				{
					hour=0;
					day++;
					if(day>days[month-1])
					{
						day=1;
						month++;
						if(month>12)
						{
							month=1;
							year++;
							
						}
					}
				}
			}
		}
	}
}


void h_m_s_second()
{
	if(m_second==1000)
	{
		m_second=0;
		ms_second++;
		if(ms_second>59)
		{
			ms_second=0;
			ms_minute++;
			if(ms_minute>9)
			{
				ms_minute=0;
			}
		}
	}
}

void display()
{
	static uchar wei=0;
	uchar temp;
	
	LED_ON=1;
	wela=1;
	P0=wela_num[wei];
	wela=0;
	
	dula=1;
	P0=0x00;
	dula=0;
	
	switch(wei)
	{
		case 0: temp=hour/10;break;
		case 1: temp=hour%10;break;
		
		case 2: temp=16;break;
		
		case 3: temp=minute/10;break;
		case 4: temp=minute%10;break;
		
		case 5: temp=16;break;
		
		case 6: temp=second/10;break;
		case 7: temp=second%10;break;
	}
	
	dula=1;
	P0=dula_num[temp];
	dula=0;

	wei++;
	if(wei>7)
		wei=0;
  P0=0xff;
	LED_ON=0;
}



void date_display()
{
	static uchar wei=0;
	uchar data_temp;
	
	LED_ON=1;
	wela=1;
	P0=wela_num[wei];
	wela=0;
	
	dula=1;
	P0=0x00;
	dula=0;
	
	switch(wei)
	{
		case 0: data_temp=year/1000;break;
		case 1: data_temp=year%1000/100;break;
		
		case 2: data_temp=year%100/10;break;
		
		case 3: data_temp=year%10;break;
		case 4: data_temp=month/10;break;
		
		case 5: data_temp=month%10;break;
		
		case 6: data_temp=day/10;break;
		case 7: data_temp=day%10;break;
	}
	
	dula=1;
	P0=dula_num[data_temp];
	dula=0;
	

	wei++;
	if(wei>7)
		wei=0;
  P0=0xff;
	LED_ON=0;
}

void c_display()
{
	static uchar wei=0;
	uchar c_temp;
	wela=1;
	P0=wela_num[wei];
	wela=0;
	
	dula=1;
	P0=0x00;
	dula=0;
	
	switch(wei)
	{
		case 0: c_temp=c_hour/10;break;
		case 1: c_temp=c_hour%10;break;
		
		case 2: c_temp=16;break;
		
		case 3: c_temp=c_minute/10;break;
		case 4: c_temp=c_minute%10;break;
		
		case 5: c_temp=16;break;
		
		case 6: c_temp=c_second/10;break;
		case 7: c_temp=c_second%10;break;
	}
	
	dula=1;
	P0=dula_num[c_temp];
	dula=0;
	
	
	wei++;
	if(wei>7)
		wei=0;
  P0=0xff;
}

void ms_display()
{
	static uchar wei=0;
	uchar ms_temp;
	
	LED_ON=1;
	
	wela=1;
	P0=wela_num[wei];
	wela=0;
	
	dula=1;
	P0=0x00;
	dula=0;
	
	switch(wei)
	{
		case 0: ms_temp=ms_minute;break;
		
		case 1: ms_temp=16;break;
		
		case 2: ms_temp=ms_second/10;break;
		case 3: ms_temp=ms_second%10;break;
		
		case 4: ms_temp=16;break;
		
		case 5: ms_temp=m_second/100;break;
		case 6: ms_temp=m_second%100/10;break;
		case 7: ms_temp=m_second%10;break;
	}
	
	dula=1;
	P0=dula_num[ms_temp];
	dula=0;
	
	wei++;
	if(wei>7)
		wei=0;
  P0=0xff;
	LED_ON=0;
}

void key_scan()
{
	static uchar status=0;
	uchar value=0;
	if(key_on==0) 
		value=1;
	else if(key_hour==0)
		value=2;
	else if(key_minute==0)
		value=3;
	else if(key_second==0)
		value=4;
	else
		value=0;
	switch(status)
	{
		case 0: if(value!=0) status++;break;
		case 1: status++;break;
		case 2: if(value!=0) {status++; key_return=value;} else status=0;break;
		case 3: if(value==0) {key_Function(); status=0;} break;
	}
}

void key_Function()
{
	switch(key_return)
	{
		case 1:
			count++;
			if(count==1)
				flag_starttime=1;
			if(count==2)
				flag_starttime=0;
			if(count==3)
				flag_starttime=0;
			if(count==4)
				flag_starttime=0;
			if(count>5)
				count=0;
			break;
		case 2: 
			if(count==1)
			{
				hour++;
					if(hour>23)
					{
		          hour=0;
							day++;
					}
			}
			if(count==2)
			{
				c_hour++;
					if(c_hour>23)
						c_hour=0;
			}
			if(count==3)
			{
				flag_startsecond=1;
			}
			if(count==4)
			{
				year++;
				if(year>2026)
					year=2016;
			}
			break;
		case 3:
			if(count==1)
			{
				minute++;
				if(minute>59)
					minute=0;
			}
			if(count==2)
			{
				c_minute++;
				if(c_minute>59)
					c_minute=0;
			}
			if(count==3)
			{
				flag_startsecond=0;
			}
			if(count==4)
			{
				month++;
				if(month>12)
					month=1;
			}
			break;
		case 4:
			if(count==1)
			{
				second++;
				if(second>59)
					second=0;
			}
			if(count==2)
			{
				c_second++;
				if(c_second>59)
					c_second=0;
			}
			if(count==3)
			{
				flag_startsecond=0;
				ms_minute=0;
				ms_second=0;
				m_second=0;
			}
			if(count==4)
			{
				day++;
				if(flag_leap==1)
				{
					if(month==2)
					{
						if(day>29)
							day=1;
					}
					else
					{
					if(day>days[month-1])
					day=1;
				  }
				}
				else
				{
					if(day>days[month-1])
					day=1;
				}
			}
			break;
		default:break;
	}
	
}

void clock()
{
	if(hour==c_hour)
	{
		if(minute==c_minute)
		{
			if(second==c_second>0)
				bee=0;
			  LED_CLOCK();
			if((second-c_second)<30)
				bee=0;
			if(key_return!=0)
				bee=1;
		}
	}

}

void LED_TIME()
{
	P0=led_time1;
	if(ms_1000>1000)
	{
		ms_1000=0;
		if(led_time1==0x00)
		{
			led_time1=0xfe;
		}
		else
		led_time1=led_time1<<1;
	}
}

void LED_CLOCK()
{
	P0=led_clock1;
	if(ms_50>50)
	{
		ms_50=0;
		led_clock1=~led_clock1;
	}
}
void LED_SEC()
{

	P0=led_sec1;
	if(ms_40>50)
	{
		ms_40=0;
		led_sec1=_crol_(led_sec1,1);
	}
}
void LED_TEM()
{
	P0=led_tem1;
	if(ms_30>1)
	{
		ms_30=0;
		led_tem1=_cror_(led_tem1,1);
	}
}


void delayus(uint t)
{
	while(t--);
}

void ds_reset()
{
	wanduan=1;
	delayus(5);
	wanduan=0;
	delayus(50);
	wanduan=1;
	delayus(14);

	delayus(20);
}

bit ds_read_bit()
{
	bit dat;
	wanduan=0;
	_nop_();
	_nop_();
	wanduan=1;
//	_nop_();
	dat=wanduan;
	delayus(10);
	return dat;
}

uchar ds_read_byte()
{
	uchar i,j,k;
	for(i=0;i<8;i++)
	{
		j=ds_read_bit();   
		k=(j<<7)|(k>>1);//1000 0000 |0100 0000
	}
	return k;
}

void ds_write_byte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		wanduan=0;
		_nop_();
		wanduan=dat&0x01;
		delayus(6);
		wanduan=1;
		dat=dat>>1;
	}
	delayus(6);
}

uint read_temperature()
{
	uchar a,b;
	ds_reset();
	ds_write_byte(0xcc);
	ds_write_byte(0xbe);
	a=ds_read_byte();
	b=ds_read_byte();
	temp=b;
	temp=temp<<8;
	temp=temp|a;
	temp=temp*0.0625*10+0.5;
	return temp;

}

void tem_display(uint temp)
{
	
	static uchar wei=0;
	uchar t_temp;
	
	LED_ON=1;
	wela=1;
	P0=wela_num[wei];
	wela=0;
	
	dula=1;
	P0=0x00;
	dula=0;
	
	switch(wei)
	{
		case 0: t_temp=temp/100;break;
		case 1: t_temp=temp%100/10;break;
		
		case 2: t_temp=temp%100%10;break;
		
	}
	
	if(wei==1)
	{
	 dula=1;
	 P0=dula_num[t_temp]|0x80;
	 dula=0;
	}
	else
	{
	dula=1;
	P0=dula_num[t_temp];
	dula=0;
	}
	
	//while(--i);
	wei++;
	if(wei>3)
		wei=0;
  P0=0xff;
	LED_ON=0;
}

void temperature()
{
	
  	ds_reset();
		ds_write_byte(0xcc);
		ds_write_byte(0x44);
//	tem_display(read_temperature());
	
}

