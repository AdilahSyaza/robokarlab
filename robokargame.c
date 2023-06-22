/*
* ROBOSAMPLE.C -- A sample/template for RoboKar program with uCOS-II
* Written by: Rosbi Mamat 6/5/2014
* Updated : 1/5/2023 Modified to show proximity & light sensor usage
*/

#include "..\inc\kernel.h" /* Always include these to use uCOS-II */
#include "..\inc\hal_robo.h" /* and RoboKar HAL */

#define TASK_STK_SZ 128 /* Size of each task's stacks (# of bytes) */
#define TASK_START_PRIO 1 /* Highest priority */
#define TASK_CHKCOLLIDE_PRIO 2
#define TASK_CTRLMOTOR_PRIO 3
#define TASK_NAVIG_PRIO 4 /* Lowest priority */

#define STOP_SPEED 0
#define LOW_SPEED 35
#define MEDIUM_SPEED 50
#define HIGH_SPEED 80

OS_STK TaskStartStk[TASK_STK_SZ]; /* TaskStartTask stack */
OS_STK ChkCollideStk[TASK_STK_SZ]; /* Task StopOnCollide stack */
OS_STK CtrlmotorStk[TASK_STK_SZ]; /* Task CtrlMotors stack */
OS_STK NavigStk[TASK_STK_SZ]; /* Task NavigRobot stack */
int count;
/* ------ Global shared variable -------*/
_Bool seven = 1;
int pastLight = 0;

/* Ideally, this should be protected by a semaphore etc */
struct robostate
{
 int rspeed; /* right motor speed (-100 -- +100) */
 int lspeed; /* leftt motor speed (-100 -- +100) */
 char obstacle; /* obstacle? 1 = yes, 0 = no */
} myrobot;

/*------High pririority task----------*/
void CheckCollision (void *data)
{
 for(;;)
 {
  if ( (robo_proxSensor() == 1) ){ /* obstacle? */
     myrobot.obstacle = 1; /* signal obstacle present */
 }else
     myrobot.obstacle = 0; /* signal no obstacle */

 OSTimeDlyHMSM(0, 0, 0, 100); /* Task period ~ 100 ms */
 }
}

/* Control robot Motors TASK */
void CntrlMotors (void *data)
{
 int speed_r, speed_l;

 for(;;)
 {
  speed_r = myrobot.rspeed;
  speed_l = myrobot.lspeed;
  robo_motorSpeed(speed_l, speed_r);
  OSTimeDlyHMSM(0, 0, 0, 10); /* Task period ~ 250 ms */
 }
}

/* --- Task for navigating robot ----
* Write you own navigation task here
*/

void Navig (void *data)
{

 for (;;)
 {
 	cprintf("%d \n", robo_lineSensor());
  if (myrobot.obstacle == 1) /* If blocked then reverse */
  {
  	if (pastLight == 1) /* If blocked then stop */
		  {
		  	myrobot.rspeed = 0; /* stop */
		   myrobot.lspeed = 0;
			robo_Honk();
			break;
		  }else{
		  myrobot.rspeed = -20; /* REVERSE */
		   myrobot.lspeed = 80;
			OSTimeDlyHMSM(0, 0, 0, 250);
		   myrobot.rspeed = 70;
			myrobot.lspeed = 70;
			OSTimeDlyHMSM(0, 0, 1, 0);
		  }
  	
   
  }else{
  robo_LED_off();
  	
	if(pastLight==1){

		switch (robo_lineSensor()) {
			case 1:	//turn right
				myrobot.rspeed = -30;          
		        myrobot.lspeed = 50;
				break;
		    case 3:	//corner right
		        myrobot.rspeed = -40;          
		        myrobot.lspeed = 50;
				break;
		    case 2:	//forward
				myrobot.rspeed = 60;          
		        myrobot.lspeed = 50;
				break;
		    case 6:	//corner left
				myrobot.rspeed = 55;          
		        myrobot.lspeed = -40;
				break;
		    case 4:	//turn left
		        myrobot.rspeed = 50;          
		        myrobot.lspeed = -30;
				break;
			case 7:
		        myrobot.rspeed = 60;       
		        myrobot.lspeed = 45;
				//printf("%d\n", robo_lineSensor());
				break;
		    case 0:	//reverse
		        myrobot.rspeed = -30;          
		        myrobot.lspeed = -50;
				//printf("%d\n", robo_lineSensor());
				break;
			default:
				//myrobot.rspeed = 50;          
		        //myrobot.lspeed = 40;
				break;
		} 

		

	}
	else{

	if (robo_lineSensor() == 2) /* go straight */
	  {
	   count = 0;
	   myrobot.rspeed = 60;
	   myrobot.lspeed = 70;
	  }else if (robo_lineSensor() == 4)
	  {
	   count = 0;
	    myrobot.rspeed = 70 ;/* turn left */
	   myrobot.lspeed =  -40;
	  }else if (robo_lineSensor() == 1)
	  {

	    myrobot.rspeed = -40; 
	   myrobot.lspeed = 70; /* turn right */

	  }else if (robo_lineSensor() == 6)
	  {

	    myrobot.rspeed = 55; 
	   myrobot.lspeed = -20; /* corner left */

	  }else if (robo_lineSensor() == 3)
	  {
	    count = 0;
	    myrobot.rspeed = -30; 
	   myrobot.lspeed = 70; /* corner right */
	  }else if (robo_lineSensor() == 0)
	  {

			myrobot.rspeed = -70; 
	    	myrobot.lspeed = -20;
	    
	  }

		else if (robo_lineSensor() == 7)
	  {
	  	myrobot.rspeed = 90; 
	   	myrobot.lspeed = 50;
		OSTimeDlyHMSM(0, 0, 0, 20);

		/*
		if(seven){
			myrobot.rspeed = 80; 
		   	myrobot.lspeed = 60; 
			seven=0;
		  }else{
		  robo_LED_on(); 
			myrobot.rspeed = 60; 
		   	myrobot.lspeed = 80; 
			seven=1;
		  }
		  */
	  }
	  else{
	  }

	}
  	  
	
	  /*else{
	  

	  if(seven){
		myrobot.rspeed = 80; 
	   	myrobot.lspeed = 60;  bias left 
		seven=0;
	  }else{
	  robo_LED_on(); 
		myrobot.rspeed = 60; 
	   	myrobot.lspeed = 80;  bias right 
		seven=1;
	  }*/
		/* myrobot.rspeed = 50; 
	   	myrobot.lspeed = 80; /* bias right */

	  
	  }
  

	// for light sensor
	
	  if (robo_lightSensor() > 80)
	  {
	   myrobot.rspeed = 70;
	   myrobot.lspeed = 10;
	   OSTimeDlyHMSM(0, 0, 0, 100);
	   pastLight = 1;
	  }else {

	  	
	  }
	  
  }
  

	OSTimeDlyHMSM(0, 0, 0, 10);
	
  /* OSTimeDlyHMSM(0, 0, 0, 10); Task period ~ 500 ms */
 }



/*------Highest pririority task----------*/
/* Create all other tasks here */
void TaskStart( void *data )
{
 OS_ticks_init(); /* enable RTOS timer tick */
	int  counter;
 OSTaskCreate(CheckCollision , /* Task function */
 (void *)0, /* nothing passed to task */
 (void *)&ChkCollideStk[TASK_STK_SZ - 1], /* stack allocated to task */
 TASK_CHKCOLLIDE_PRIO); /* priority of task */

 OSTaskCreate(CntrlMotors , /* Task function */
 (void *)0, /* nothing passed to task */
 (void *)&CtrlmotorStk[TASK_STK_SZ - 1], /* stack allocated to task */
 TASK_CTRLMOTOR_PRIO); /* priority of task */

 OSTaskCreate(Navig, /* Task function */
 (void *)&counter, /* nothing passed to task */
 (void *)&NavigStk[TASK_STK_SZ - 1], /* stack allocated to task */
 TASK_NAVIG_PRIO); /* priority of task */

 while(1)
 {
  OSTimeDlyHMSM(0, 0, 5, 0); /* Task period ~ 5 secs, argument position: HOURS, MINUTES, SECONDS, and MILLISECONDS S*/ 
  robo_LED_toggle(); /* Show that we are alive */
 }

}

int main( void )
{
 robo_Setup(); /* initialize HAL for RoboKar */
 OSInit(); /* initialize UCOS-II kernel */

 robo_motorSpeed(STOP_SPEED, STOP_SPEED); /* Stop the robot, value STOP_SPEED == 0 */
 myrobot.rspeed = STOP_SPEED; /* Initialize myrobot states */
 myrobot.lspeed = STOP_SPEED;
 myrobot.obstacle = 0; /* No collision */
 count =0;
 OSTaskCreate(TaskStart, /* create TaskStart Task */
 (void *)0,
 (void *)&TaskStartStk[TASK_STK_SZ - 1],
 TASK_START_PRIO);
 robo_Honk(); robo_wait4goPress(); /* Wait for to GO */
 OSStart(); /* Start multitasking */
 while (1); /* die here */
}
