//contains all the code
#include <stdint.h>
#include <stdbool.h>

#define TEST_IMG_2_SIZE (240*640)
//background
#define X_MAX 320
#define Y_MAX 240
//bow location
#define bow_X_MAX 30
#define bow_Y_MAX 30
#define bow_X_UPDATE 30
#define bow_Y_UPDATE 30
//target location
#define tar_X_MAX 30
#define tar_Y_MAX 30
//scoreboard constants 
#define zero_X 40
#define zero_Y 40
#define one_X 40
#define one_Y 40
#define two_X 40
#define two_Y 38
#define three_X 40
#define three_Y 40

//the bow intial location
int bowLocX = 0;
int bowLocY = 0;

//location of the target
int tarLocX = 160;
int tarLocY = 120;

//location of the score board
int X = -5;
int Y = 200;

//access next level game
bool levelOne = false;
bool levelTwo = false;
bool levelThree = false;
bool levelFour = false;

//game ends
bool endpage_four = false;

//track of the wins
int score = 0;

//winning a level each
bool wonLevel;
	
//functions
volatile int pixel_buffer_start; 
void plot_pixel(int x, int y, short int line_color);
void wait_for_vsync ();
void swap(int *xp, int *yp);
void clear_screen();

//draw the different pages
void draw_start_page(void);
void draw_win_page(void);
void draw_lose_page(void);

//draws background with no bow
void draw_background_no_bow_target(void);

//identifies location of bow
void draw_bow(int bowLocX, int bowLocY);

//update position of the bow
void update_bow_pos(void);

//location of the target you would like to hit 
void target_location(int tarLocX, int tarLocY);

//check if the bow has hit the target 
bool aiming_check_target_levelone(void);

//check if the bow has hit the target 
bool aiming_check_target_leveltwo(void);

//check if the bow has hit the target 
bool aiming_check_target_levelthree(void);

//check if the bow has hit the target 
bool aiming_check_target_levelfour(void);

//checking for the level and what target to display next
bool check_level(void);
void target_level();


//score boards
void scoreZeroBoard(void);
void scoreOneBoard(void);
void scoreTwoBoard(void);
void scoreThirdBoard(void);

int main(void){
    
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    
	/* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    
	/* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
   
   // we draw back buffer
	pixel_buffer_start = *(pixel_ctrl_ptr + 1); 
	
	//draw the start page 
	draw_start_page();
	
	// code for updating the locations of boxes (not shown)
	wait_for_vsync(); // swap front and back buffers on VGA vertical sync
	pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	
	// delay loop for start page to stay 
	for (int delay_count = 5000000; delay_count != 0; --delay_count);
	
	//forever loop, because we never exit the game 
	//screen only changes based of input 
	while(1){
		
		//draw the background
		draw_background_no_bow_target();
		
		//draw the target, defined by changing levels
		target_level();
		
		//draw the bow target 
		draw_bow(bowLocX, bowLocY);
		
		//intial score is zero
		scoreZeroBoard();
		//change the current score 
		if(score == 1){
			scoreOneBoard();	
		}else if (score == 2){
			scoreTwoBoard();
		}else if (score == 3){
			scoreThirdBoard();
			//won the game condition
		}else if (endpage_four == true && score == 4){
			// code for updating the locations of boxes (not shown)
	
			clear_screen();

			draw_win_page();
			
			wait_for_vsync(); // swap front and back buffers on VGA vertical sync
			pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
			
			//delay so you can see the sreen responce 
			for (int delay_count = 5000000; delay_count != 0; --delay_count);
			
			//exit game
			break;
		}
		
		//You lost, did not score all the points 
		if (endpage_four == true && score != 4){
			
			clear_screen();
			draw_lose_page();

			// code for updating the locations of boxes (not shown)
			wait_for_vsync(); // swap front and back buffers on VGA vertical sync
			pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
			
			//delay so you can see the screen response
			for (int delay_count = 5000000; delay_count != 0; --delay_count);

			//exit the game now
			break;
		}
		
		// code for updating the locations of boxes (not shown)
		wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
		
		//user can change location of bow 
		update_bow_pos();
		
		//level check 
		bool wonLevel = check_level();
		
		if(wonLevel){
			score = score + 1;
		}	
	}
}

bool check_level(){
	//levelOne 
	if(levelOne == false && levelTwo == false && levelThree == false){
		return aiming_check_target_levelone();
	//levelTwo			
	}else if(levelOne == true && levelTwo == false && levelThree == false){
		return aiming_check_target_leveltwo();
	//levelThree			
	}else if(levelOne == true && levelTwo == true && levelThree == false){
		return aiming_check_target_levelthree();
	//levelFour
	}else if(levelThree == true && levelOne == true && levelTwo == true){
		return aiming_check_target_levelfour();
	}
	
	return false;
}

void target_level(void){
	//levelOne
	if(levelOne == false && levelTwo == false && levelThree == false){
		//location of the target
		tarLocX = 120;
		tarLocY = 120;
		target_location(tarLocX, tarLocY);
	//levelTwo			
	}else if(levelOne == true && levelTwo == false && levelThree == false){
		//location of the target
		tarLocX = 90;
		tarLocY = 120;
		target_location(tarLocX, tarLocY);
	//levelThree			
	}else if(levelOne == true && levelTwo == true && levelThree == false){
		//location of the target
		tarLocX = 30;
		tarLocY = 90;
		target_location(tarLocX, tarLocY);
	//levelFour			
	}else if(levelThree == true && levelOne == true && levelTwo == true){
		//location of the target
		tarLocX = 120;
		tarLocY = 120;
		target_location(tarLocX, tarLocY);
	}
}

bool aiming_check_target_levelone(void){
	//SW slider switch address
	volatile int * SW_switch_ptr = (int *)0xFF200040;
	
	//Read the SW slider switch values
	int SW_value = *(SW_switch_ptr);
	
	//condition that checks if they've won/lost
	//only allowable if SW 0 is enabled
	if(SW_value == 1){

		//allows you to move to the next level
		levelOne = true;
		
		if(((tarLocX == bowLocX) && (tarLocY == bowLocY))){
			//coordinates are within position
			//win
			return true;
		}else{
			//coordinates are outisde position
			//lost
			return false;
		}
	}
	
	return false;
	
}

bool aiming_check_target_leveltwo(void){
	//SW slider switch address
	volatile int * SW_switch_ptr = (int *)0xFF200040;
	
	//Read the SW slider switch values
	int SW_value = *(SW_switch_ptr);
	
	//condition that checks if they've won/lost
	//only allowable if SW 1 is enabled
	if(SW_value == 2){
		
		//allows you to move to the next level
		levelTwo = true;
		
		if(((tarLocX == bowLocX) && (tarLocY == bowLocY))){
			//coordinates are within position
			//win
			return true;
		}else{
			//coordinates are outisde position
			//lost
			return false;
		}
	}
	
	return false;
	
}

bool aiming_check_target_levelthree(void){
	//SW slider switch address
	volatile int * SW_switch_ptr = (int *)0xFF200040;
	
	//Read the SW slider switch values
	int SW_value = *(SW_switch_ptr);
	
	//condition that checks if they've won/lost
	//only allowable if SW 2 is enabled
	if(SW_value == 4){
		
		//allows you to move to the next level
		levelThree = true;
		
		if(((tarLocX == bowLocX) && (tarLocY == bowLocY))){
			//coordinates are within position
			//win
			return true;
		}else{
			//coordinates are outisde position
			//lost
			return false;
		}
	}
	
	return false;
	
}

bool aiming_check_target_levelfour(void){
	//SW slider switch address
	volatile int * SW_switch_ptr = (int *)0xFF200040;
	
	//Read the SW slider switch values
	int SW_value = *(SW_switch_ptr);
	
	//condition that checks if they've won/lost
	//only allowable if SW 3 is enabled
	if(SW_value == 8){ 

		//game ends
		endpage_four = true; 
		
		if(((tarLocX == bowLocX) && (tarLocY == bowLocY))){
			//coordinates are within position
			//win
			return true;
		}else{
			//coordinates are outisde position
			//lost
			return false;
		}
	}
	
	return false;
	
}

void target_location(int tarLocX, int tarLocY){
	//to access the bytes 
	int k = 0;
	
	//colunms (y)
	for(int i = tarLocY; i < tarLocY + tar_Y_MAX; i++){
		//rows (x)) 
		for(int j = tarLocX; j < tarLocX + tar_X_MAX; j++){
			//reading 2 bytes per pixel in each iteration
			short int pixel = (location_ToThrow_map[k + 1] << 8) | location_ToThrow_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}
}

void update_bow_pos(void){
	//read the key by register 
	//Address for all KEYS
	volatile int * keys_addr = (int *)0XFF200050;
	
	//Reads edge capture(catches any change, key presed) register for keys
	//The capture register is at offset 3 in terms of int, that is, 4th int.
	volatile int * readKey = (int *) (keys_addr + 3);
	
	//reading keys 
	int captureKey = *readKey;
	
	//derefence readKey to read the capture flag 
	if(captureKey != 0){
		//reset capture reg to 0
		*readKey = captureKey;
		
		//if key 0 was pressed, bow goes right   
		if(captureKey & 1){
			//to go right, current location should increase by x pixel size 
			//if the location is at max (320), do not update target location
			if(bowLocX + bow_X_UPDATE <= X_MAX){
				//update location of the x
				bowLocX = bowLocX + bow_X_UPDATE;
			}
		}
		
		//if key 1 was pressed, bow goes left 
		if(captureKey & 2){
			//to go left, current location should decrease by x pixel size 
			//if the location is at min (0), do not update target location
			if(bowLocX - bow_X_UPDATE >= 0){
				//update location of the x
				bowLocX = bowLocX - bow_X_UPDATE;
			}
		}
		
		//if key 2 was pressed, bow goes down  
		if(captureKey & 4){
			//to move down, y_loc should be decreased 
			//if the location is at min (0), do not update target location				
			if(bowLocY - bow_Y_UPDATE >= 0){
				//update location of the y
				bowLocY = bowLocY - bow_Y_UPDATE;
			}
		}
		
		//if key 3 was pressed, bow goes up 
		if(captureKey & 8){
			//to move upward, the y_loc should be increased
			//if the location is at max (240), do not update target location
			if(bowLocY + bow_Y_UPDATE <= Y_MAX){
				//update location of the y
				bowLocY = bowLocY + bow_Y_UPDATE;
			}
		}
	}
}

void draw_start_page(void){
	//to access the bytes 
	int k = 0;
	
	//colunms (y)
	for(int i = 0; i < Y_MAX; i++){
		//rows (x)) 
		for(int j = 0; j < X_MAX; j++){
			short int pixel = (STARTPAGE_map[k + 1] << 8) | STARTPAGE_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}
}

void draw_win_page(void){
	//colunms (y)
	int k = 0;
	for(int i = 0; i < Y_MAX; i++){
		//rows (x)) 
		for(int j = 0; j < X_MAX; j++){
			short int pixel = (WINPAGE_map[k + 1] << 8) | WINPAGE_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}
}

void draw_lose_page(void){
	//colunms (y)
	int k = 0;
	for(int i = 0; i < Y_MAX; i++){
		//rows (x) 
		for(int j = 0; j < X_MAX; j++){
			short int pixel = (LOSEPAGE_map[k + 1] << 8) | LOSEPAGE_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
			 
		}
	}
}

void scoreZeroBoard(void){
	int k = 0;

	//colunms (y)
	for(int i = Y; i < Y + zero_Y; i++){
		//rows (x)) 
		for(int j = X; j < X + zero_X; j++){
			//reading 2 bytes per pixel in each iteration
			short int pixel = (zero_map[k + 1] << 8) | zero_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}	
}

void scoreOneBoard(void){
	int k = 0;

	//colunms (y)
	for(int i = Y; i < Y + one_Y; i++){
		//rows (x)) 
		for(int j = X; j < X + one_X; j++){
			//reading 2 bytes per pixel in each iteration
			short int pixel = (one_map[k + 1] << 8) | one_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}	
}

void scoreTwoBoard(void){
	
	//colunms (y)
	int k = 0;
	for(int i = Y; i < Y + two_Y; i++){
		//rows (x)) 
		for(int j = X; j < X + two_X; j++){
			//reading 2 bytes per pixel in each iteration
			short int pixel = (two_map[k + 1] << 8) | two_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}	
}

void scoreThirdBoard(void){
	
	//colunms (y)
	int k = 0;
	for(int i = Y; i < Y + three_Y; i++){
		//rows (x)) 
		for(int j = X; j < X + three_X; j++){
			//reading 2 bytes per pixel in each iteration
			short int pixel = (three_map[k + 1] << 8) | three_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}	
}

void draw_background_no_bow_target(void){
    
	//to access the bytes 
	int k = 0;
	
	//colunms (y)
	for(int i = 0; i < Y_MAX; i++){
		//rows (x)) 
		for(int j = 0; j < X_MAX; j++){
			//reading 2 bytes per pixel in each iteration
			short int pixel = (backNOTARGET_map[k + 1] << 8) | backNOTARGET_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}	
}

void draw_bow(int bowLocX, int bowLocY){
	//to access the bytes 
	int k = 0;
	
	//colunms (y)
	for(int i = bowLocY; i < bow_Y_MAX + bowLocY; i++){
		//rows (x)) 
		for(int j = bowLocX; j < bow_X_MAX + bowLocX; j++){
			//reading 2 bytes per pixel in each iteration
			short int pixel = (bowLocation_map[k + 1] << 8) | bowLocation_map[k];
			plot_pixel(j, i, pixel);
			k = k + 2;
		}
	}
}

void wait_for_vsync () {
	
	//holds the addres of the front buffer
	//default address buffer (somewhere) in memory reads from 
	volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
	int status;
	
	//requesting a swap here
	*(pixel_ctrl_ptr) = 1;
	
	//read the status bit, will check swapping flag 
	status = *(pixel_ctrl_ptr + 3);
	
	//runs swapping
	while ( (status & 0x01) != 0) 
	{
		status = *(pixel_ctrl_ptr +3);
	}
}

void swap(int *xp, int *yp){ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void plot_pixel(int x, int y, short int line_color){
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

// code not shown for clear_screen() 
void clear_screen(){
	//rows (x)
	for(int i = 0; i < X_MAX; i++){
		//colunms (y) 
		for(int j = 0; j < Y_MAX; j++){
			plot_pixel(i, j, 0); 
		}
	}
    
}
	
	
	