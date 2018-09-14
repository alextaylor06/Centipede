#include <windows.h>  //must be included to work in windows.
#include <math.h>     
#include <stdlib.h>	 
#include <stdio.h> 	
#include <time.h>	
#include <string.h>
#include <a:\c\CentipedeFinal\glut.h>  //connects to glut.h 



#define NUMSHROOMS 55 //number of mushrooms minus 10
#define STEP 0.18 //step size for each space, one "block" on screen is a square with length STEP
#define SCREEENXBOUND 22*STEP
#define RIGHTLIMIT (STEP*20)
#define LEFTLIMIT (STEP*-20)
#define BOTTOMLIMIT STEP
#define TOPLIMIT (4*STEP)
#define SCREENTOP 19*STEP
#define NUMBODY 10 //number of centipede segments
void drawShrooms();  		//Draws shrooms and centipede bodies
void drawPlayer();  		//Draws player at player location
void drawBullet();			//draws bullet
void initShrooms();   		//sets initial shroom and body properties
void checkHit();			//checks bullet hits
void drawSpider();			//draws spider
void updateSpider();		//determines spider movement
void processNormalKeys(unsigned char,int,int);   //Space bar fires, esc exits.
void specialKeys(int,int,int); //arrows move the defender
void updateCent();			//movement of centipede
int getEtime(void);          // number of ms since last frame
void Loser();				//runs if player loses
void Winner();				//runs if all enemies defeated
void death();				//checks for death and hits 
void LoseAnimation();		//animation for losing
void renderBitmapString(float,float,char*);	//displays text
void music();//plays background music

int statusShroom[NUMSHROOMS]={0};
float xShroom[NUMSHROOMS]={0};
float yShroom[NUMSHROOMS]={0};
int ShroomDirection=0;
int bodyDir[NUMBODY]={0};
int down[NUMBODY]={0};
int difficultyLevel=1;


char text[] = "000000";
char *text1 = text ;
int score1 = 0;

float xBullet=0;
float yBullet = 0;
float xPlayer = 0.0;
float yPlayer=0.12;
float xSpider=-10*STEP;
float ySpider=13*STEP;
float xbullet=0.0;
float ybullet = 0.0;
float totalTime=0.0;
float vxPlayer = 0.0;
float vyPlayer=0.0;

time_t timerStart;
time_t timerStop;
time_t timerMove;
int statusSpider=0;
int etime = 0;
int ydirection=1;
int xscalar=1;
int yscalar=1;
int change=1;
int right=0;
int left=0;
int top=0;
int bottom=0;
int over=0;
int sound=1;
int musac=1;
int lastime = -105050;
void changeSize(int w, int h) {
    //DO NOT CHANGE ANYTHING IN THIS FUNCTION.
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
    
	int row;
	int col;
	
	
	//You may change the following line to adjust the background:
	glClearColor(0.0f, 0.0f, 0.0f, 0); //Set background to black
	
	// Clear the screen. 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	// Reset transformations. 
	glLoadIdentity(); 
	// Set the camera view. 
	gluLookAt(0.0f, 2.3f, 5.5, //Location of your eye
			  0.0f, 2.3f, 0.0f, //Location you are looking at
			  0.0f, 1.0f, 0.0f); //Direction of "up"
		
	if(over==0){     // If player is alive run everything
	etime = getEtime();
	updateCent();
	updateSpider();
	drawPlayer();
	drawBullet();
	drawShrooms();
	drawSpider();
	checkHit();
	music();
	renderBitmapString(-13*STEP,22*STEP,text1);
	death();
	}
	if(over==1)  // If player is dead run death function repeatedly
	{
		death();
	}
	glutSwapBuffers();
} 
void renderBitmapString(float x, float y,char *text){
	int i=0;
	glPushMatrix();
	glLineWidth(3);
	glTranslatef((float)x,(float)y,1);
	glColor3f(1.0,1.0,1.0);
	glScalef(0.0009,0.0009,0.0009);
	for (i=0; i<6; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,  (int)text[i]);
	}
	glPopMatrix();
}
void music(){
	int time = glutGet(GLUT_ELAPSED_TIME);
	if((time-lastime)<=106020&&(time-lastime)>=105080)
	{														//plays background music every 1:45
		PlaySound("background1.wav" ,NULL, SND_ASYNC);
		lastime=time;
	}
}
void updateCent(){
	float speed=.001*difficultyLevel;
	int i=0;
	for(i=0; i<NUMSHROOMS; i++)
	{
		if(statusShroom[i]==4)//is still a centipede body it will move
		{
			if(bodyDir[i]==-1)//moves left
			{
				xShroom[i]-=speed*etime;
			}
			if(bodyDir[i]==1)//moves right
			{
				xShroom[i]+=speed*etime;
			}
			if((xShroom[i]<LEFTLIMIT)&&bodyDir[i]==-1)//moves down and determines what side it just hit
			{
				bodyDir[i]=1;
				yShroom[i]-=(down[i])*STEP;
				
				
			}
			if(xShroom[i]>RIGHTLIMIT&&bodyDir[i]==1)//moves down and determines what side it just hit
			{
				bodyDir[i]=-1;
				yShroom[i]-=(down[i])*STEP;
				
			}
			if(yShroom[i]<(1*STEP))//determined bottom and top bounds for centipede and when to bounce off
			{
				down[i]=-1;
			}
			if(yShroom[i]>(23*STEP))
			{
				down[i]=1;
			}
		}
		int j=0;
		for(j=0;j<NUMSHROOMS;j++){
			if(statusShroom[j]!=4 && statusShroom[j]!=0 && bodyDir[i]==1 && statusShroom[i]==4 &&(sqrt(pow(xShroom[i]-xShroom[j],2)+pow(yShroom[i]-yShroom[j],2)+pow(0-0,2))<0.05))
			{							//checks if body hits shroom thats still alive and moves it down to the next level
				bodyDir[i]=-1;
				yShroom[i]-=(down[i])*STEP;
				
				
			}
			if(statusShroom[j]!=4 && statusShroom[j]!=0 && bodyDir[i]==-1 && statusShroom[i]==4 &&(sqrt(pow(xShroom[i]-xShroom[j],2)+pow(yShroom[i]-yShroom[j],2)+pow(0-0,2))<0.05))
			{						//checks if body hits shroom thats still alive and moves it down to the next level for opposite side
				bodyDir[i]=1;
				yShroom[i]-=(down[i])*STEP;
			}
		}
	}
	
}
void death(){
	int i=0;
	int numBodies=0;
		for(i=0; i<NUMSHROOMS; i++)												
	{
		if(statusShroom[i]==4 &&(sqrt(pow(xShroom[i]-xPlayer,2)+pow(yShroom[i]-yPlayer,2)+pow(0-0,2))<0.1))
			{	
				LoseAnimation();	//runs lose animation and loser function if centpede body hits player
				Loser();
				over=1;
			}
			if(statusShroom[i]==4)		//counts number of centipede left
		{
			numBodies++;
		}
		if((sqrt(pow(xSpider-xPlayer,2)+pow(ySpider-yPlayer,2)+pow(0-0,2))<0.15) && statusSpider==0)
	{
		LoseAnimation();			//same as last loser loop
		Loser();
		over=1;
	}
	}
	if(numBodies==0)		//if all centipede is killed you win
		{
			Winner();
			over=1;
		}
}
void updateSpider(){
	float speed=.0015*difficultyLevel;
	
	if(change==1)				//Spider will change everytime it hits a wall, it remembers what side it it and will not choose the next vector in the same direction
	{							//Will only change once when it hits a wall
		if(right==1)			//just hit right wall and will pick a direction not to the right
		{
			yscalar=rand()%(200)-100.0;
			xscalar=rand()%(100)-100.0;
		}
		if(left==1)
		{
			yscalar=rand()%(200)-100.0;
			xscalar=rand()%(100);
		}
		if(bottom==1)														//Just hit the wall and will not schoose a vector in the same direction as the wall it just hit
		{
			xscalar=rand()%(200)-100.0;
			yscalar=rand()%(100);
			
		}
		if(top==1)
		{
			xscalar=rand()%(200)-100.0;
			yscalar=rand()%(100)-100.0;
		}
		
	change=0;
	right=0;
	left=0;
	bottom=0;
	top=0;
	}
	xSpider+=speed*etime*xscalar*.01;	//determines its direction and speed off the random variables just generated
	ySpider+=speed*etime*yscalar*.01;
	
	if((sqrt(pow(xSpider-xBullet,2)+pow(ySpider-yBullet,2)+pow(0-0,2))<0.2) && statusSpider==0)//checks if spider is hit by bullet
		{
			statusSpider=1;
			yBullet=0;
			score1+=(500*(difficultyLevel*.5));
		}
	if(xSpider<LEFTLIMIT&&change==0 && left==0)		//checks which wall it hits and stores it to determine next direction
	{
		change=1;
		left=1;
		
	}
	if(xSpider>RIGHTLIMIT&&change==0 && right==0)
	{
		change=1;
		right=1;
	}
	if(ySpider<(BOTTOMLIMIT)&&change==0 && bottom==0)
	{
		change=1;
		bottom=1;
	
	}
	if(ySpider>(14*STEP)&&change==0 && top==0)
	{
		change=1;
		top=1;
	}
	
}
void checkHit(){
	int i=0;
	int score = score1;
	for (i=0; i<NUMSHROOMS; i++)
	{
		if(statusShroom[i]==0) continue;
		if(statusShroom[i]==1 && (sqrt(pow(xShroom[i]-xBullet,2)+pow(yShroom[i]-yBullet,2)+pow(0-0,2))<0.1))
		{
			statusShroom[i]=2;																					//all if loops check what state it is in when it is hit and sends it the the correct next state
			yBullet=0;
		}
		if(statusShroom[i]==2 && (sqrt(pow(xShroom[i]-xBullet,2)+pow(yShroom[i]-yBullet,2)+pow(0-0,2))<0.1))
		{
			statusShroom[i]=3;
			yBullet=0;
		}
		if(statusShroom[i]==3 && (sqrt(pow(xShroom[i]-xBullet,2)+pow(yShroom[i]-yBullet,2)+pow(0-0,2))<0.1))
		{
			score1+=(10*(difficultyLevel*.5));
			statusShroom[i]=0;																						//Destroying a mushroom completely gives you score
			yBullet=0;
		}
			if(statusShroom[i]==4 && (sqrt(pow(xShroom[i]-xBullet,2)+pow(yShroom[i]-yBullet,2)+pow(0-0,2))<0.1))
		{
			statusShroom[i]=1;
			yBullet=0;																						//Destroying a centipede body gives you score
			score1+=(50*(difficultyLevel*.5));
		}
	}
		text[0] = score/100000 +'0';
		score = score%100000;
		text[1] = score/10000 +'0';
		score = score%10000;
		text[2] =score/1000 +'0';
		score = score%1000;
		text[3] = score/100 +'0';
		score = score%100;
		text[4] = score/10 +'0';
		score = score%10;
		text[5] = score +'0';
}	
int getEtime(void) {
  static int lasttime = 0;
  int time = glutGet(GLUT_ELAPSED_TIME);
  int etime = time - lasttime;
  lasttime = time; // store current time for next frame
  return (etime);
}
void drawPlayer(){
	if ((xPlayer>=LEFTLIMIT)&&(vxPlayer<0)) xPlayer+=vxPlayer*etime;			//determines player movement from the keys that are being pressed
	if ((xPlayer<=RIGHTLIMIT)&&(vxPlayer>0)) xPlayer+=vxPlayer*etime;
	if ((yPlayer>=BOTTOMLIMIT)&&(vyPlayer<0)) yPlayer+=vyPlayer*etime;
	if ((yPlayer<=TOPLIMIT)&&(vyPlayer>0)) yPlayer+=vyPlayer*etime;
	
	glPushMatrix();
 	glTranslatef(xPlayer, yPlayer, 0);			//draws player at location it has already determined
	float color1=.3;
	float color2=.8;
	float color3=.28;
	float color4=.7;
	float color5=.2;
	float color6=.7;
	if(difficultyLevel==10) //special player if secret level 10 is on
	{
		color1=.9;
		color2=.1;
		color3=.9;
		color4=0;
		color5=1;
		color6=1;
	}
	glColor3f(color1,color2,color3);
	glTranslatef(-.03f, .045f, 0);
    glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(-.09f, -.03f, 0);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glColor3f(color4,color5,color6);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glColor3f(color1,color2,color3);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glColor3f(color4,color5,color6);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glColor3f(color1,color2,color3);
	glutSolidCube(0.03f);
	glTranslatef(-.12f, -.03f, 0);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(-.09f, -.03, 0);
    glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.03f);
	glTranslatef(-.03f, 0.12f, 0);
	glutSolidCube(0.03f);
	glTranslatef(.0f, 0.02f, 0);
	glColor3f(color4,color5,color6);
	glutSolidCube(0.03f);
	glPopMatrix();
	
}
void drawSpider(){
	glPushMatrix();
	if(statusSpider==0)
	{
		glTranslatef(xSpider, ySpider, 0);			//draws spider if it is alive
		glColor3f(0.3f,.8f,0.28f);
		
		glScalef(.8,.8,.8);
		glutSolidCube(0.03f);
		glTranslatef(.03f, .0, 0);
		glutSolidCube(0.03f);
		glTranslatef(-.06f, -.03f, 0);
		glColor3f(0.8f,.2f,0.28f);
		glutSolidCube(0.03f);
		glColor3f(0.3f,.8f,0.28f);
		glTranslatef(.03f, .0, 0);
		glutSolidCube(0.03f);
		glTranslatef(.03f, .0, 0);
		glutSolidCube(0.03f);
		glTranslatef(.03f, .0, 0);
		glColor3f(0.8f,.2f,0.28f);
		glutSolidCube(0.03f);
		glColor3f(0.3f,.8f,0.28f);
		glTranslatef(0, -.03f, 0);
		glutSolidCube(0.03f);
		glTranslatef(-.03f, .0, 0);
		glutSolidCube(0.03f);
		glTranslatef(-.03f, .0, 0);
		glutSolidCube(0.03f);
		glTranslatef(-.03f, .0, 0);
		glutSolidCube(0.03f);
		glTranslatef(.03f, -.03f, 0);
		glutSolidCube(0.03f);
		glTranslatef(.03f, .0, 0);
		glutSolidCube(0.03f);
		
		glColor3f(0.9f,.9f,0.9f);
		glTranslatef(.05f, .03f, 0);
		glutSolidCube(0.02f);
		glTranslatef(.02f, .02f, 0);
		glutSolidCube(0.02f);
		glTranslatef(.02f, -.02f, 0);
		glutSolidCube(0.02f);
		glTranslatef(.02f, -.02f, 0);
		glutSolidCube(0.02f);
		
		glTranslatef(-.08f, .09f, 0);
		glutSolidCube(0.02f);
		glTranslatef(.02f, .02f, 0);
		glutSolidCube(0.02f);
		glTranslatef(.02f, -.02f, 0);
		glutSolidCube(0.02f);
		glTranslatef(.02f, -.02f, 0);
		glutSolidCube(0.02f);
		
		glTranslatef(-.15f, 0.02f,0);
		glutSolidCube(0.02f);
		glTranslatef(-.02f, 0.02f,0);
		glutSolidCube(0.02f);
		glTranslatef(-.02f, -0.02f,0);
		glutSolidCube(0.02f);
		glTranslatef(-.02f, -0.02f,0);
		glutSolidCube(0.02f);
		
		glTranslatef(.03f, -0.055f,0);
		glutSolidCube(0.02f);
		glTranslatef(-.02f, 0.02f,0);
		glutSolidCube(0.02f);
		glTranslatef(-.02f, -0.02f,0);
		glutSolidCube(0.02f);
		glTranslatef(-.02f, -0.02f,0);
		glutSolidCube(0.02f);
		glPopMatrix();
	}
}
void drawBullet(){
	
	if(yBullet>0)				//draws bullet when space is hit, spawns at player location 
	{
		yBullet+=0.01 * etime;
		if(yBullet>5){
			yBullet=0;
		}
		glPushMatrix();
		glTranslatef(xBullet, yBullet+.09f, 0);
		glColor3f(0.7f,.2f,0.7f);
		glRectf(-0.03f,-0.05f, 0.0, 0.0f);
		glPopMatrix();
	}
}
void specialKeys(int key, int xx, int yy) {

  const float VELOCITY = 0.005f;
  switch (key) {
  case GLUT_KEY_LEFT : vxPlayer = -1.0*VELOCITY; break;//will move in direction of arrow key when hit
  case GLUT_KEY_RIGHT : vxPlayer = VELOCITY; break;			 
  case GLUT_KEY_UP : vyPlayer = VELOCITY; break;
  case GLUT_KEY_DOWN : vyPlayer = -1.0*VELOCITY; break;
  }
} 
void processNormalKeys(unsigned char key, int xx, int yy) { 	

    if (key == 27)  exit(0); //exit if escape key is it
	if (key == 32)		//will shoot if space is hit
	{		
		yBullet=yPlayer+.05;
		xBullet=xPlayer;
		
	}
} 
void releaseKey(int key, int x, int y) { 	
 
	switch (key) {
		case GLUT_KEY_LEFT : vxPlayer =0; break; //will stop moving when keys are let go
		case GLUT_KEY_RIGHT :vxPlayer =0; break;			 
		case GLUT_KEY_UP :vyPlayer =0; break;
		case GLUT_KEY_DOWN : vyPlayer =0; break;
    }
} 
void drawShrooms(){
	int i=0;
	
	for (i=0; i<NUMSHROOMS; i++)
	{
		if(statusShroom[i]==0) continue; //if shroom is dead dont draw
		if(statusShroom[i]==1)
		{
			glPushMatrix();
			glTranslatef(xShroom[i], yShroom[i]-.04, 0);//draw state 1 complete shroom
			glScalef(.9,.9,.9);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(-.015f, .105f, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.06f, -.03f, 0);
			glutSolidCube(0.03f);
			glColor3f(0.3f,.8f,0.28f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.12f, -.03f, 0);
			glutSolidCube(0.03f);
			glColor3f(0.3f,.8f,0.28f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.15f, -.02f, -.0001);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.09f, -.03f, 0);
			glColor3f(0.3f,.8f,0.28f);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.03f, -.03f, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glPopMatrix();
		}
		if(statusShroom[i]==2) // draw hit mushroom state 2
		{
			glPushMatrix();
			glTranslatef(xShroom[i], yShroom[i]-.04, 0);
			glScalef(.9,.9,.9);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(-.015f, .105f, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.06f, -.03f, 0);
			glutSolidCube(0.03f);
			glColor3f(0.3f,.8f,0.28f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.12f, -.03f, 0);
			
			glColor3f(0.3f,.8f,0.28f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.15f, -.02f, -.0001);
			
			glTranslatef(.03f, 0, 0);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glPopMatrix();
		}
		if(statusShroom[i]==3)
		{												//draw almost destroyed shroom in state 3
			glPushMatrix();
			glTranslatef(xShroom[i], yShroom[i]-.04, 0);
			glScalef(.9,.9,.9);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(-.015f, .105f, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(-.06f, -.03f, 0);
			glutSolidCube(0.03f);
			glColor3f(0.3f,.8f,0.28f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glTranslatef(.03f, 0, 0);
			glColor3f(0.8f,.2f,0.2f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(0.03f);
			glPopMatrix();
		}
		if(statusShroom[i]==4)
		{								//draw centipede body, not a shroom
			glPushMatrix();
			glTranslatef(xShroom[i], yShroom[i], 0);
			
			glScalef(.9, .9, .9);
			glColor3f(0.2f,.4f,0.1f);
			glTranslatef(-.015f, .06f, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(.03f);
			glTranslatef(-.06f, -.03f, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(.03f);
			glTranslatef(-.09f, -.03f, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f, 0, 0);
			glutSolidCube(.03f);
			glTranslatef(-.06f, -.03f, 0);
			glutSolidCube(.03f);
			glTranslatef(.03f,0, 0);
			glutSolidCube(.03f);
			if(bodyDir[i]==1)//will face right if going right
			{
				glTranslatef(.045f, -.015f, 0);
			}
			if(bodyDir[i]==-1)//faces left when going left
			{
				glTranslatef(-.075f, -.015f, 0);
			}
			glColor3f(0.8f,.9f,0.0f);
			glutSolidCube(.025f);
			glTranslatef(.0, .12f, 0);
			glutSolidCube(.025f);
			
			glPopMatrix();
		}
	}
}	
void initShrooms(){     //finds position of Shroom mushroom
	int i=0, j=0, k=0, ShroomNum=0;
	float colors[3]={1, 1, 1};

	for (i=0; i<NUMSHROOMS-NUMBODY; i++) {
		
				statusShroom[ShroomNum]=1;
				xShroom[ShroomNum]=(rand()%SCREEENXBOUND -rand()%SCREEENXBOUND);
				yShroom[ShroomNum]=((STEP)+TOPLIMIT)+rand()%SCREENTOP;
				ShroomNum++;
	}
	while(i<NUMSHROOMS)
	{
		statusShroom[i]=4;
		bodyDir[i]=1;
		down[i]=1;
		xShroom[i]=(1*STEP*(i-(NUMSHROOMS)));
		yShroom[i]=(24*STEP);
		ShroomNum++;
		i++;
	}
}
void Loser(){
	char Sorry[]="Game Over";
	char *charPtr=NULL;
	if(sound==1){
	PlaySound("wilhelm.wav" ,NULL, SND_ASYNC);//plays sound and displays loser text
	sound=0;
	}
	glPushMatrix();
	glLineWidth(3);
	glTranslatef(-5*STEP,14*STEP,1);
	glColor3f(0.0,1.0,1.0);
	glScalef(0.002,0.002,0.002);
	for(charPtr=Sorry; *charPtr; charPtr++)
	{
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *charPtr);
	}
	glPopMatrix();
	glutSwapBuffers();
	glutKeyboardFunc(processNormalKeys);
}
void Winner(){
char Winner[]="WINNER!!";
	char *charPtr=NULL;
	glPushMatrix();
	glLineWidth(3);
	if(sound==1){
	PlaySound("victory.wav" ,NULL, SND_ASYNC);//plays sound and displays loser text
	sound=0;
	}
    glTranslatef(-4*STEP,14*STEP,1);
	glColor3f(0.0f,0.9f,.9f);
	glScalef(0.002,0.002,0.002);
	for(charPtr=Winner; *charPtr; charPtr++)
	{
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *charPtr);
	}
	glPopMatrix();
	glutSwapBuffers();
	glutKeyboardFunc(processNormalKeys);
}
void LoseAnimation(){						
	glPushMatrix();
 	glTranslatef(xPlayer, yPlayer, 0);
	glColor3f(0.0f,.0f,0.0f);					//makes player disappear
	glTranslatef(-.03f, 0.045f, 0);
    glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05f);
	glTranslatef(-.09f, -.03f, 0);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glColor3f(0.0f,.0f,0.0f);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glColor3f(0.0f,.0f,0.0f);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glColor3f(0.0f,.0f,0.0f);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glColor3f(0.0f,.0f,0.0f);
	glutSolidCube(0.05f);
	glTranslatef(-.12f, -.03f, 0);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05f);
	glTranslatef(-.09f, -.03, 0);
    glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05f);
	glTranslatef(.03f, 0, 0);
	glutSolidCube(0.05f);
	glTranslatef(-.03f, 0.12f, 0);
	glutSolidCube(0.05f);
	glTranslatef(.0f, 0.02f, 0);
	glColor3f(0.0f,.0f,0.0f);
	glutSolidCube(0.05f);
	glPopMatrix();
}
void InitScene(void){
  GLfloat LightAmbient[]=	{ 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat LightDiffuse[]=	{ 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };

  // Set the fogcolor
  GLfloat fogColor[4] = {0.0f,0.0f,0.0f,1.0f};		/* Fog Color */
  GLfloat fogStartDepth = 15.0f; // Distance where fog should START
  GLfloat fogEndDepth = 25.0f; // Distance where fog ends (its all fog)

  GLfloat backColor[4] = {0.0f,0.0f,0.0f,1.0f};		/* Clear (background) Color */
 
  //Takes care of Initializing OpenGL parameters
  glShadeModel(GL_SMOOTH);		/* Enable Smooth Shading */

  glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]); // Set the background color

  glClearDepth(1.0f);			/* Depth Buffer Setup */
  glEnable(GL_DEPTH_TEST);		/* Enables Depth Testing */
  glDepthFunc(GL_LEQUAL);			/* The Type Of Depth Testing To Do */
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	/* Really Nice Perspective Calculations */

  // SETUP LIGHTING
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		/* Setup The Ambient Light */
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		/* Setup The Diffuse Light */
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	/* Position The Light */

  glEnable(GL_LIGHT1);					/* Enable Light One */
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  // Choose 1: (It's how the fog is displayed)
  //glFogi(GL_FOG_MODE, GL_EXP);	/* Fog Mode */
  //glFogi(GL_FOG_MODE, GL_EXP2);	/* Fog Mode */
  glFogi(GL_FOG_MODE, GL_LINEAR);	/* Fog Mode */
  
  // Set fogcolor
  glFogfv(GL_FOG_COLOR, fogColor);		/* Set Fog Color */
  glFogf(GL_FOG_DENSITY, 0.35f);			/* How Dense Will The Fog Be */

  glHint(GL_FOG_HINT, GL_DONT_CARE);		/* Fog Hint Value */
  // How far away should we START showing fog
  glFogf(GL_FOG_START, fogStartDepth);			/* Fog Start Depth */
  // At this distance, everything should be fog
  glFogf(GL_FOG_END, fogEndDepth);			/* Fog End Depth */
  glEnable(GL_FOG);				/* Enables GL_FOG */

}
int main(int argc, char **argv) {
  
	timerStart=clock();
	difficultyLevel=atoi(argv[1]);//finds difficulty level
	if (argv[1]==NULL){
		difficultyLevel=3;//defaults to 3
	}
	if (difficultyLevel<1) difficultyLevel=1;//sets difficulty to 1 if under
	if (difficultyLevel>5 && difficultyLevel!=10) difficultyLevel=5;//sets difficulty to 5 if over 5, except 10
	printf("Difficulty Level is %d", difficultyLevel);
	srand(time(NULL));
	initShrooms();//initiates shrooms
	
	
	// init GLUT and create window
	glutInit(&argc, argv); //Allows input arguments to initialize OpenGL.
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(480,175); //Create window at this location.
	glutInitWindowSize(800,800); //Default window size.
	glutCreateWindow("Centipede");  //Window title.

	// register callbacks
	glutDisplayFunc(renderScene);  //When you need to display, call renderScene().
	glutReshapeFunc(changeSize);   //When you need to reshape, call changeSize().
	glutIdleFunc(renderScene);
	  

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(specialKeys);
	glutSpecialUpFunc(releaseKey);

	//OpenGL init
	InitScene();
	// OpenGL init
	glEnable(GL_DEPTH_TEST);  //Enables objects to appear in 3D

	// enter GLUT event processing cycle
	glutMainLoop();  //Enter an infinite loop to draw the picture.
	
	return 1;
}
