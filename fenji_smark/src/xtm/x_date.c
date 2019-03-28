#include "includes.h"

#include "stdlib.h"

#define x_date_h

#define CommonH
#include "common.h"

/*
****************************************
201310
新增时间函数
****************************************
*/

void NextDate();   //下一个时间
//--------------------------------------------------------------------------
void NextDate()
{
	int mon[12]={31,28,31,30,31,30,31,31,30,31,30,31};

	/*if (DataTime.year<=0 || 
		DataTime.month<=0 ||
		DataTime.month>12 ||
		DataTime.day<0 || 
		DataTime.day>31 || 
		DataTime.hour<0 || 
		DataTime.hour>24 || 
		DataTime.minute<0 || 
		DataTime.minute>=60 ||
		DataTime.second<0 || 
		DataTime.second>=60)
	{
		return ;
	}

	if (DataTime.year%400==0||DataTime.year%100!=0&&DataTime.year%4==0)
	{
		mon[1] = 29;
	}

	DataTime.minute++;

	if (DataTime.minute>=60)
	{
		DataTime.minute=0;
		DataTime.hour++;
		if (DataTime.hour>=24)
		{
			DataTime.hour = 0;
			DataTime.day++;
			if (DataTime.day>mon[DataTime.month-1])
			{

				DataTime.day=1;

				DataTime.month++;

				if (DataTime.month>12)
				{
					DataTime.month=1;
					DataTime.year++;
				}

			}
		}
	}*/


				//strcpy(date, "" );
				//itoa(DataTime.year, temp, 10);
				//strcat( date, temp );
				//strcat(date,"-");
				//itoa(DataTime.month, temp, 10);
				//strcat( date, temp );
				//strcat(date,"-");
				//itoa(DataTime.day, temp, 10);
				//strcat( date, temp );
				
				//strcat(date," ");

				//itoa(DataTime.hour, temp, 10);
				//strcat( date, temp );
				//strcat(date,":");
				//itoa(DataTime.minute, temp, 10);
				//strcat( date, temp );

				//RefreshBackWindow(REFRESH_TOP, 0); //刷新背景

                //DisplayImage(&top_image, FROMFILE, 0);
		        //DisplayImage(&logo_image[0], FROMFILE, 0);
				
			    //display_text(0, 10, 0, COLOR_WHITE, date, 0, 10, NOFULLPAGE);

}




