// Define enviromental variables.
#define SCREEN_HEIGHT 				127
#define SCREEN_WIDTH 				127
#define SCREEN_CENTER				63

#define RACKET_THICKNESS 			6
#define RACKET_LENGTH 				48

#define RACKET_LEFT_PERIOD 			100
#define RACKET_LEFT_LIMIT_X_LEFT 	RACKET_THICKNESS/2
#define RACKET_LEFT_LIMIT_X_RIGHT 	RACKET_THICKNESS + RACKET_THICKNESS/2
#define RACKET_LEFT_LIMIT_Y_UP 		SCREEN_HEIGHT - RACKET_LENGTH/2
#define RACKET_LEFT_LIMIT_Y_DOWN 	RACKET_LENGTH/2
#define RACKET_LEFT_POS_X			(RACKET_LEFT_LIMIT_X_LEFT + RACKET_LEFT_LIMIT_X_RIGHT)/2
#define RACKET_LEFT_POS_Y			SCREEN_CENTER
//
#define RACKET_RIGHT_PERIOD 		100
#define RACKET_RIGHT_LIMIT_X_LEFT 	SCREEN_WIDTH - (RACKET_THICKNESS + RACKET_THICKNESS/2) //121
#define RACKET_RIGHT_LIMIT_X_RIGHT 	SCREEN_WIDTH - RACKET_THICKNESS/2 // 124
#define RACKET_RIGHT_LIMIT_Y_UP 	SCREEN_HEIGHT - RACKET_LENGTH/2
#define RACKET_RIGHT_LIMIT_Y_DOWN 	RACKET_LENGTH/2
#define RACKET_RIGHT_POS_X			(RACKET_RIGHT_LIMIT_X_LEFT + RACKET_RIGHT_LIMIT_X_RIGHT)/2
#define RACKET_RIGHT_POS_Y			SCREEN_CENTER
//
//
#define BALL_PERIOD					100
#define BALL_THICKNESS 				RACKET_THICKNESS
#define BALL_LIMIT_X_RIGHT			122
#define BALL_LIMIT_X_LEFT			BALL_THICKNESS
#define BALL_LIMIT_Y_UP				SCREEN_HEIGHT - RACKET_LENGTH/2
#define BALL_LIMIT_Y_DOWN			BALL_THICKNESS/2
#define BALL_INIT_POS_X				RACKET_LEFT_POS_X+BALL_THICKNESS/2
#define BALL_INIT_POS_Y				RACKET_LEFT_POS_Y
