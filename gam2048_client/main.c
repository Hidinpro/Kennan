#include <stdio.h>
#include <stdlib.h>
#include "game2048.h"

int main(int argc, char *argv[]) 
{
	init_game2048();
	
	start_game2048();
	
	exit_game2048();
	
	return 0;
}
