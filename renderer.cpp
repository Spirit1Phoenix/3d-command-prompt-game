#include <iostream>
#include <conio.h>  // Include the conio.h header for _getch() function on Windows
#include <windows.h>
#include <thread>
#include <cmath>
#include <chrono>



//game variables
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std; //just so i dont need to write std every fucking time
int hres; //horizontal resolution
int vres; //vertical resolution
bool gamerun = true;
int runtime = 0;
int prevrun;
bool rendering = true;
int* selectedarray;



//player based variables
int playerxpos; //x position on the array
int playerypos; //y position on the array
char playerfacing[4] = {'n','e','s','w'}; //0 for north, 1 for east, 2 for south, 3 for west.
int targetx; //x position of target
int targety; //y position of target
int guntype[6] = {3,5,7,20,16,0}; //first value is damage, second is clip size, third is firing speed, fourth is reload speed, fifth is damage falloff percent, sixth is the model id



void printtotemplate(int north, int east, int south, int west, int layer, char character) { //generates template on a specific layer
	switch(layer) {
		case 0:
		selectedarray = rendertemplateA1;
		break;
		
		case 1:
		selectedarray = rendertemplateA2;
		break;
		
		case 3:
		selectedarray = rendertemplateA3;
		break;
		
		case 4:
		selectedarray = rendertemplateB1;
		break;
		
		case 5:
		selectedarray = rendertemplateB2;
		break;
		
		case 6:
		selectedarray = rendertemplateB3;
		break;
		
		case 7:
		selectedarray = rendertemplateC1;
		break;
		
		case 8:
		selectedarray = rendertemplateC2;
		break;
		
		case 9:
		selectedarray = rendertemplateC3;
		break;
		
		case 10:
		selectedarray = rendertemplateD1;
		break;
		
		case 11:
		selectedarray = rendertemplateD2;
		break;
		
		case 12:
		selectedarray = rendertemplateD3;
		break;
		
		case 13:
		selectedarray = rendertemplateE1;
		break;
		
		case 14:
		selectedarray = rendertemplateE2;
		break;
		
		case 15:
		selectedarray = rendertemplateE3;
		break;
		
		case 16:
		selectedarray = rendertemplateF1;
		break;
		
		case 17:
		selectedarray = rendertemplateF2;
		break;
		
		case 18:
		selectedarray = rendertemplateF3;
		break;
	};
		
	for(int i; i <= vres; i++) {
		for(int o; o <= hres; o++) {
			if(i >= north && i <= south && o >= west && o <= east) {
				selectedarray[i][o] = character;
			};
		};
	};
}

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
	
	
	
}


	
void printtrapezoidtemplate(int north1, int east1, int south1, int west1, bool lefteright, int layer, char character) {
	int tempx = east1 - west1;//get x relative position
	int tempy = south1 - north1;//get y relative position
	bool inverted;
	int riseoverrun;
	if(tempx <= tempy) { //get the amount of 
		riseoverrun = tempy / tempx;
		inverted = true; 
		// if the angle is more down than right
		
	} else {
		riseoverrun = tempx / tempy;
		inverted = false;
		// if the angle is more right than down
	};
	int pythagorusinput1 = pow(tempx, 2);
	int pythagorusinput2 = pow(tempy, 2);
	int pythagorusinput3 = pythagorusinput1 + pythagorusinput2;
	int pythagorusoutput = sqrt(pythagorusinput3);
	pythagorusinput1 = 0;
	pythagorusinput2 = 0;
	pythagorusinput3 = 0;
	
	for(int brug = 0; brug <= pythagorusoutput; brug++) {
		if(lefteright = true) {
		if(inverted == true) {
			
		if(pythagorusinput1 == riseoverrun) {
			pythagorusinput1 = 0;
			pythagorusinput2++;
		};
		printtotemplate(north1 + pythagorusinput2, west1 + pythagorusinput1, vres - north1, west1 + pythagorusinput1, layer, character)
		pythagorusinput1++;
		} else {
		if(pythagorusinput1 == riseoverrun) {
			pythagorusinput1 = 0;
			pythagorusinput2++;
		};
		printtotemplate(north1 + pythagorusinput1, west1 + pythagorusinput2, vres - north1, west1 + pythagorusinput2, layer, character)
		pythagorusinput1++;
		};
	};
	} else {
		riseoverrun = -riseoverrun
		if(inverted == true) {
			
		if(pythagorusinput1 == riseoverrun) {
			pythagorusinput1 = 0;
			pythagorusinput2--;
		};
		printtotemplate(north1 + pythagorusinput2, west1 + pythagorusinput1, vres - north1, west1 + pythagorusinput1, layer, character)
		pythagorusinput1--;
		} else {
		if(pythagorusinput1 == riseoverrun) {
			pythagorusinput1 = 0;
			pythagorusinput2--;
		};
		printtotemplate(north1 + pythagorusinput1, west1 + pythagorusinput2, vres - north1, west1 + pythagorusinput2, layer, character)
		pythagorusinput1--;
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
		if(gamemap[checky][checkx] == 'e') {
			checky--;
		} else {
			hitting = true;
		};
	};
	break;

	case 'e':
	while(!hitting) {
		if(gamemap[checky][checkx] == 'e') {
			checkx++;
		} else {
			hitting = true;
		};
	};
	break;
	
	case 's':
	while(!hitting) {
		if(gamemap[checky][checkx] == 'e') {
			checky++;
		} else {
			hitting = true;
		};
	};
	break;
	
	case 'w':
	while(!hitting) {
		if(gamemap[checky][checkx] == 'e') {
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



char gamemap[5][5] = {
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
		runtime++;//rising edge
		this_thread::sleep_for(milliseconds(33));
		runtime2++;//falling edge
		this_thread::sleep_for(milliseconds(33));
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
	char rendertemplateA1[vres][hres] = {'.'};
	char rendertemplateA2[vres][hres] = {'.'};
	char rendertemplateA3[vres][hres] = {'.'};
	char rendertemplateB1[vres][hres] = {'.'};
	char rendertemplateB2[vres][hres] = {'.'};
	char rendertemplateB3[vres][hres] = {'.'};
	char rendertemplateC1[vres][hres] = {'.'};
	char rendertemplateC2[vres][hres] = {'.'};
	char rendertemplateC3[vres][hres] = {'.'};
	char rendertemplateD1[vres][hres] = {'.'};
	char rendertemplateD2[vres][hres] = {'.'};
	char rendertemplateD3[vres][hres] = {'.'};
	char rendertemplateE1[vres][hres] = {'.'};
	char rendertemplateE2[vres][hres] = {'.'};
	char rendertemplateE3[vres][hres] = {'.'};
	char rendertemplateF1[vres][hres] = {'.'};
	char rendertemplateF2[vres][hres] = {'.'};
	char rendertemplateF3[vres][hres] = {'.'};
    engine();
	
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