#include <iostream>
#include <conio.h>  // Include the conio.h header for _getch() function on Windows
#include <windows.h>
#include <thread>



//game variables
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std; //just so i dont need to write std every fucking time
int hres; //horizontal resolution
int vres; //vertical resolution
bool gamerun = true;
int runtime = 0;
int prevrun;
bool rendering = true;




//player based variables
int playerxpos; //x position on the array
int playerypos; //y position on the array
char playerfacing[4] = {'n','e','s','w'} //0 for north, 1 for east, 2 for south, 3 for west.
int targetx; //x position of target
int targety; //y position of target
int guntype[4] = {3,5,7,20,16,0} //first value is damage, second is clip size, third is firing speed, fourth is reload speed, fifth is damage falloff percent, sixth is the model id


//rendering templates
void settempplates() {
	/*  player fov
	a[1][2][3]
	b[ ][ ][ ]
	c[ ][ ][ ]
	d[ ][ ][ ]
	e[ ] p [ ]
	*/
	int midx = hres / 2;
	int midy = vres / 2;
	//a1 wall/portal tile
	int westvert = midx / 4;
	int plcvert = midy / 5;
	int northvert = midy - plcvert;
	int eastvert = midx - westvert;
	int southvert = midy + plcvert;
	printtotemplate(northvert, eastvert, southvert, westvert, 0, '+');
	
	
	
};

void printtotemplate(int north, int east, int south, int west, int layer, char character) {
	for(int i; i <= vres; i++) {
		for(int o; o <= hres; o++) {
			if(i >= north && i <= south && o >= west && o <= east) {
				rendertemplates[i][o][layer] = character;
			};
		};
	};
}
	
	
	

void lookingat() {
	bool hitting = false;
	int checkx = playerxpos;
	int checky = playerypos;
	
	switch(playerfacing[face]) {
	case 'n':
	while(!hitting) {
		if(map[checky][checkx] == 'e') {
			checky--;
		} else {
			hitting = true;
		};
	};
	break;

	case 'e':
	while(!hitting) {
		if(map[checky][checkx] == 'e') {
			checkx++;
		} else {
			hitting = true;
		};
	};
	break;
	
	case 's':
	while(!hitting) {
		if(map[checky][checkx] == 'e') {
			checky++;
		} else {
			hitting = true;
		};
	};
	break;
	
	case 'w':
	while(!hitting) {
		if(map[checky][checkx] == 'e') {
			checkx--;
		} else {
			hitting = true;
		};
	};
	break;
	};
	
	targetx = checkx;
	targety = checky;
};

void gunlogic() {
	


};



char map[5][5] = {
	{'w','w','e','w','w'},
	{'w','w','e','w','w'},
	{'e','e','e','e','e'},
	{'w','w','e','w','w'},
	{'w','w','w','w','w'}
};


void engine() {
	
	thread clock(gameclock);
	thread externalscr(renderscreen);
	thread internalscr(composscreen);
};



void gameclock() { //runs internal clock to sync everything to
	bool clockrun = true;
	while(clockrun) {
		runtime++;
		sleep(66);
	};
}


void composscreen() {
	
	//resets internal screen to entirely transparent + black
	for(int IntScreenVertical = 0; IntScreenVertical <= vres; IntScreenVertical++) {
		for(int IntScreenHorizontal = 0; IntScreenHorizontal<= hres; IntScreenHorizontal++) {
			screen[IntScreenVertical][IntScreenHorizontal] = ' ';
			colours[IntScreenVertical][IntScreenHorizontal] = 0;
		};
	};
	
	
	
	
}
	




void renderscreen() {
		while(gamerun) {
		while(runtime == prevrun) {
				sleep(10);
		};
		
		system("CLS");//clears screen
		prevrun = runtime;
	for(int i = 0; i <= vres; i++) { //renders the entire screen according to the resolution 
		for(int o = 0; o <= hres; o++) {
			SetConsoleTextAttribute(hConsole, colours[i][o]);
			cout<<screen[i][o];
		};
		cout<<endl;
		};

	};
};

void setres() {
	while(true) {
	cout<<"please select your resolution\n";
	cout<<"the default resolution is 118 across by 28 high, or 167 by 42\n";
	cout<<"if you enter something that isnt a number, any errors that happen are on you, restart the program nerd\n";
	cout<<"please input width\n";
	cin>>hres;
	cout<<"please input height\n";
	cin>>vres;
	cout<<"now testing resolution\n";
	
	Sleep(3000); //sleep for 3 seconds and then clear screen
	system("CLS");
	
	for(int u = 0; u <= vres; u++) {
		for(int y = 0; y <= hres; y++) {
			if(u == 0||u == vres||y == 0||y == hres) {
				SetConsoleTextAttribute(hConsole, 4);
			} else {
				SetConsoleTextAttribute(hConsole, 7);
			};
			cout<<"#";
		};
		cout<<endl;
	};
	SetConsoleTextAttribute(hConsole, 7);
	
	Sleep(5000);
	system("CLS");
	

	cout<<"the entire game will be rendered within that border\n";
	cout<<"enter y to proceed to the game, or press anything else to enter a new resolution\n";
	char randomchar = getch();
	if(randomchar == 'y') {
		break;
	};
	};
	
	
}

	
	
	


int main() {
    char input;
	
	setres();

    
    std::cout << "Type any key. Press 'q' to quit." << std::endl;
	
	
	
	char screen[vres][hres];
	int colours[vres][hres];
	
	//templates init
	char rendertemplates[vres][hres][14] = {NULL}
    
    while (true) {
        if (_kbhit()) {  // Check if a key has been pressed
            input = _getch();  // Read the key that was pressed
            
            if (input == 'q')
                break;  // Exit the loop if 'q' is pressed
            system("CLS");
		};
            std::cout << "You pressed: " << input << endl;
			
        }
		
        
        // Perform other tasks or update the program state here
    }










//raptley if you are reading this i love you <3