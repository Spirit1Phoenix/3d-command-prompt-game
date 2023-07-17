#include <iostream>
#include <conio.h>  // Include the conio.h header for _getch() function on Windows
#include <windows.h>
#include <cmath>
#include <chrono>
#include <time.h>
#include <thread>
#include <fstream>


//templates for rendering
int templates[300][100][13] = {0};

//game variables
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std; //just so i dont need to write std every fucking time
int hres; //horizontal resolution
int vres; //vertical resolution
int pracvres;
bool gamerun = true;
int runtime = 0;
int runtime2 = 0;
int prevrun;
bool rendering = true;
char screen[300][100];
int colours[300][100];
	int mapx;
	int mapy;


//player based variables
int playerxpos; //x position on the array
int playerypos; //y position on the array
int face;
char playerfacing[4] = {'n','e','s','w'}; //0 for north, 1 for east, 2 for south, 3 for west.
int targetx; //x position of target
int targety; //y position of target
int guntype[6] = {3,5,7,20,16,0}; //first value is damage, second is clip size, third is firing speed, fourth is reload speed, fifth is damage falloff percent, sixth is the model id






char gamemap[50][50] = {'w'};

void log(const std::string& message) { //chatgpt logging base
    std::ofstream logfile("log.txt", std::ios::app);  // Open log file in append mode
    if (logfile.is_open()) {
        logfile << message << std::endl;
        logfile.close();
    } else {
        std::cerr << "Failed to open log file." << std::endl;
    }
}

void generatemap (int startx, int starty) {
	//set entire map to walls
	int snakegen;
	int snakelengthrand;
	string snakexy;
	for(int mapresety = 0; mapresety < 50; mapresety++) {
		for(int mapresetx = 0; mapresetx < 50; mapresetx++) {
			gamemap[mapresetx][mapresety] = 'W';
		};
	};
	int nextmovecontrol = 0;
	//int mapx = startx;
	//int mapy = starty;
	mapy = starty;
	mapx = startx;

	for(int snakeai = 0; snakeai < 100; snakeai++) {
		snakegen = (rand()%100);
		snakelengthrand = 0;
		
		if(snakegen < 50) {
			nextmovecontrol--;
		} else if(snakegen > 50) {
			nextmovecontrol++;
		};
		
		if(nextmovecontrol == 4) {
			nextmovecontrol = 0;
		} else if(nextmovecontrol == -1) {
			nextmovecontrol = 3;
		};
		while(snakelengthrand < 6) {
		
		snakelengthrand = (rand()%10);
		
		switch(nextmovecontrol) {
			case 0:
			mapy--;
			break;
				
			case 1:
			mapx++;
			break;
			
			case 2:
			mapy++;
			break;
			
			case 3:
			mapx--;
			break;
		};
		log("x coord");
		snakexy = to_string(mapx);
		log(snakexy);
		log("y coord");
		snakexy = to_string(mapy);
		log(snakexy);
		if(mapx >=46) {
			mapx = 4;
		} else if(mapx <= 3) {
			mapx = 45;
		};
		if(mapy >= 46) {
			mapy = 4;
		} else if(mapy <= 3) {
			mapy = 45;
		};
		gamemap[mapx][mapy] = ' ';
		};
	
};
gamemap[mapx][mapy] = '#';
gamemap[startx][starty] = 's';
}


void templateprinter(int north, int east, int south, int west, int layer, int colourselect) {
	for(int o = 0; o < vres; o++) {
		for(int i = 0; i < hres; i++) {
			if(i <= east && i >= west && o <= south && o >= north) {
				templates[i][o][layer] = colourselect;
			};
		};
	};
};


void printtraptemplate(int x1, int y1, int x2, int y2, int layer2, bool updown, int coloursel) {
	bool invert = false;
	int tempx = x2 - x1;
	int tempy = y2 - y1;
	int xovery = tempx / tempy;
	int northern = y1;
	int secondary = 0;
	int eastern = x2;
	int western = x1;
	cout<<"western:"<<western<<endl;
	if(tempx <= tempy) {
		xovery = tempy / tempx;
		invert = true;
	};
	int pythagorusoutput = (sqrt((pow(tempx, 2)) + (pow(tempy, 2)))) - 1;
	int seconddown = 1;
	cout<<"tempx:"<<tempx<<endl;
	cout<<"tempy:"<<tempy<<endl;
	cout<<"xovery:"<<xovery<<endl;
	cout<<"pythagorusoutput:"<<pythagorusoutput<<endl;
	cout<<"inverted:"<<invert<<endl;
	if(updown == true) {
		if(invert == false) {
			for(int bruh = 0; bruh < pythagorusoutput; bruh++) {
	cout<<"bruh:"<<bruh<<endl;

	
				if(secondary == xovery) {
					seconddown++;
					secondary = 0;
				};
				int frot = northern + seconddown;
				int balls = vres - frot;
				western++;
				
				templateprinter(frot, western, balls, western, layer2, coloursel);
				secondary++;
					cout<<"frot:"<<frot<<endl;
	cout<<"balls:"<<balls<<endl;
	cout<<"western:"<<western<<endl;
	cout<<"secondary:"<<secondary<<endl;
			};
		};
	} else {
		for(int bruh = 0; bruh < pythagorusoutput; bruh++) {
			if(secondary == xovery) {
				seconddown++;
				secondary = 0;
			};
			int frot = northern + seconddown;
			int balls = vres - frot;
			eastern--;
			templateprinter(frot, eastern, balls, eastern, layer2, coloursel);
			secondary++;
		};
	};
		
	
}

//rendering templates
void settempplates() {
	cout<<"setting templates\n";
	/*  player fov
	a[1][2][3][4][5]
	b   [ ][ ][ ]
	c   [ ][ ][ ]
	d   [ ][ ][ ]
	e   [ ] p [ ]
	*/
	int midx = hres / 2;
	int midy = vres / 2;
	//a1 wall/portal tile
	int westvert = midx / 4;
	int plcvert2 = midx / 4;
	int plcvert = midy / 5;
	int northvert = midy - plcvert;
	int eastvert = midx - westvert;
	int southvert = midy + plcvert;
	int plcvert3 = eastvert;
	int trapvert1 = northvert;
	int trapvert2 = eastvert;
	templateprinter(northvert, eastvert, southvert, westvert, 0, 1); //a2
	templateprinter(northvert, westvert, southvert, 0, 3, 1); //a1
	//a3 wall/portal tile
	eastvert = hres - westvert;
	westvert = midx + westvert;
	int trapvert3 = westvert;
	templateprinter(northvert, eastvert, southvert, westvert, 1, 1); //a4
	templateprinter(northvert, hres, southvert, eastvert, 4, 1); //a5
	westvert = midx - plcvert2;
	eastvert = midx + plcvert2;
	templateprinter(northvert, eastvert, southvert, westvert, 2, 1); //a3
	plcvert = midy / 3.5;
	plcvert2 = midx / 2.5;
	westvert = hres / 20;
	northvert = midy - plcvert;
	southvert = midy + plcvert;
	eastvert = midx - plcvert2;
	int plcvert4 = eastvert;
	templateprinter(northvert, eastvert, southvert, westvert, 5, 2); //b2
	
	printtraptemplate(eastvert, northvert, trapvert2, trapvert1, 5, true, 3);
	
	trapvert2 = northvert;
	eastvert = hres - westvert;
	westvert = midx + plcvert2;
	templateprinter(northvert, eastvert, southvert, westvert, 6, 2); //b4
	printtraptemplate(trapvert3, northvert, westvert, trapvert1, 6, false, 3);
	trapvert1 = eastvert;
	templateprinter(northvert, westvert, southvert, plcvert, 7, 2); //b3
	
}


	


void lookingat() {
	bool hitting = false;
	int checkx = playerxpos;
	int checky = playerypos;
	
	switch(playerfacing[face]) {
	case 'n':
	while(!hitting) {
		if(checky <= 3) {
			checky = 45;
		};
		if(gamemap[checky][checkx] == 'e') {
			checky--;
		} else {
			hitting = true;
		};
	};
	break;

	case 'e':
	while(!hitting) {
		if(checkx >= 46) {
			checkx = 4;
		};
		if(gamemap[checky][checkx] == 'e') {
			checkx++;
		} else {
			hitting = true;
		};
	};
	break;
	
	case 's':
	while(!hitting) {
		if(checky >= 46) {
			checky = 4;
		};
		if(gamemap[checky][checkx] == 'e') {
			checky++;
		} else {
			hitting = true;
		};
	};
	break;
	
	case 'w':
	while(!hitting) {
		if(checkx <= 3) {
			checkx = 45;
		};
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









void gameclock() { //runs internal clock to sync everything to
	bool clockrun = true;
	while(clockrun) {
		runtime++;//rising edge
		this_thread::sleep_for(chrono::milliseconds(33));
		runtime2++;//falling edge
		this_thread::sleep_for(chrono::milliseconds(33));
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
				this_thread::sleep_for(chrono::milliseconds(10));
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

void engine() {
	
	thread clock(gameclock);
	thread externalscr(renderscreen);
	thread internalscr(composscreen);
};
	
	


int main() {
    char input;


	//setres();
vres = 28;
hres = 118;
    system("PAUSE");
	auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Type any key. Press 'q' to quit." << std::endl;
	system("CLS");
	srand(time(NULL));
	
	settempplates();
    //generatemap(25, 25);
	for(int harold = 0; harold < 13; harold++) {
	for(int james = 0; james < vres; james++) {
		for( int bob = 0; bob < hres; bob++) {
			/*SetConsoleTextAttribute(hConsole, 4);
			if(bob == 3||bob == 46||james == 3||james == 46) {
				SetConsoleTextAttribute(hConsole, 5);
			};
			if(gamemap[bob][james] != 'W') {
				SetConsoleTextAttribute(hConsole, 3);
			}; */
			cout<<templates[bob][james][harold];
		};
		cout<<endl;
	};
	cout<<endl<<endl;
	};
	cout<<"portal is at "<<mapx<<", "<<mapy<<endl;
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	
	system("PAUSE");
    /*while (true) {
        if (_kbhit()) {  // Check if a key has been pressed
            input = _getch();  // Read the key that was pressed
            
            if (input == 'q')
                break;  // Exit the loop if 'q' is pressed
            system("CLS");
		};
            std::cout << "You pressed: " << input << endl;
			
        }
		*/
        
        // Perform other tasks or update the program state here
    }











//raptley if you are reading this i love you <3