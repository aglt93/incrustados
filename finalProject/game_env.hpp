// Define enviromental variables.
#define SCREEN_HEIGHT 				127
#define SCREEN_WIDTH 				127
#define SCREEN_CENTER				63

#define RACKET_THICKNESS 			6
#define RACKET_LENGTH 				48

#define RACKET_LEFT_PERIOD 			99
#define RACKET_LEFT_LIMIT_X_LEFT 	RACKET_THICKNESS/2 // 3
#define RACKET_LEFT_LIMIT_X_RIGHT 	RACKET_THICKNESS + RACKET_THICKNESS/2 // 9
#define RACKET_LEFT_LIMIT_Y_DOWN	SCREEN_HEIGHT - RACKET_LENGTH/2 // 103
#define RACKET_LEFT_LIMIT_Y_UP	 	RACKET_LENGTH/2 // 24
#define RACKET_LEFT_POS_X			(RACKET_LEFT_LIMIT_X_LEFT + RACKET_LEFT_LIMIT_X_RIGHT)/2
#define RACKET_LEFT_POS_Y			SCREEN_CENTER // 63
//
#define RACKET_RIGHT_PERIOD 		100
#define RACKET_RIGHT_LIMIT_X_LEFT 	SCREEN_WIDTH - (RACKET_THICKNESS + RACKET_THICKNESS/2) //118
#define RACKET_RIGHT_LIMIT_X_RIGHT 	SCREEN_WIDTH - RACKET_THICKNESS/2 // 124
#define RACKET_RIGHT_LIMIT_Y_DOWN 	SCREEN_HEIGHT - RACKET_LENGTH/2 // 103
#define RACKET_RIGHT_LIMIT_Y_UP 	RACKET_LENGTH/2 // 24
#define RACKET_RIGHT_POS_X			(RACKET_RIGHT_LIMIT_X_LEFT + RACKET_RIGHT_LIMIT_X_RIGHT)/2 // 121
#define RACKET_RIGHT_POS_Y			SCREEN_CENTER // 63
//
//
#define BALL_PERIOD					65
#define BALL_THICKNESS 				RACKET_THICKNESS // 6
#define BALL_LIMIT_X_RIGHT			RACKET_RIGHT_LIMIT_X_LEFT - BALL_THICKNESS/2 - 1 // 114
#define BALL_LIMIT_X_LEFT			RACKET_LEFT_LIMIT_X_RIGHT + BALL_THICKNESS/2 + 1 // 13
#define BALL_LIMIT_Y_UP				BALL_THICKNESS/2 // 3
#define BALL_LIMIT_Y_DOWN			SCREEN_HEIGHT - BALL_THICKNESS/2 // 124
#define BALL_INIT_POS_X				RACKET_LEFT_LIMIT_X_RIGHT + BALL_THICKNESS/2 + 1 // 13
#define BALL_INIT_POS_Y				RACKET_LEFT_POS_Y // 63
//
#define LOGIC_PERIOD				100
