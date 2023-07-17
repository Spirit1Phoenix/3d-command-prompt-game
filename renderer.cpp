#include <iostream>
#include <conio.h>  // Include the conio.h header for _getch() function on Windows
#include <windows.h>
#include <cmath>
#include <chrono>
#include <time.h>
#include <thread>
#include <fstream>


//templates for rendering
int templates[300][100][20] = {0};

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
int screen[300][100];
char gamemap[50][50] = {'w'};
int mapx;
int mapy;


//player based variables
int playerxpos; //x position on the array
int playerypos; //y position on the array
int face = 0;
char playerfacing[4] = {'n','e','s','w'}; //0 for north, 1 for east, 2 for south, 3 for west.
int targetx; //x position of target
int targety; //y position of target
int guntype[6] = {3,5,7,20,16,0}; //first value is damage, second is clip size, third is firing speed, fourth is reload speed, fifth is damage falloff percent, sixth is the model id

//render based variables
int wallexist[5][4] = {0};
int checkwallx = 0;
int checkwally = 0;

void setCursorPosition(int x, int y) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(consoleHandle, pos);
}

int wraparoundcheckshort(int wrapy) {
	
	if(wrapy == 3) {
	wrapy = 45;
	} else if(wrapy == 2) {
		wrapy = 44;
	} else if(wrapy == 46) {
		wrapy = 4;
	} else if(wrapy == 47) {
		wrapy = 5;
	};
	return wrapy;
}

int wraparoundchecklong(int wrap) {
	switch(wrap) {
		case 3:
		wrap = 45;
		break;
		case 2:
		wrap = 44;
		break;
		case 1:
		wrap = 43;
		break;
		case 0:
		wrap = 42;
		break;
		case 46:
		wrap = 4;
		break;
		case 47:
		wrap = 5;
		break;
		case 48:
		wrap = 6;
		break;
		case 49:
		wrap = 7;
		break;
		default:
		wrap = wrap;
		break;
	};
	return wrap;	
}

void checkwallexist() {
	for(int wallcheckv = 0; wallcheckv < 5; wallcheckv++) {
		for(int wallcheckh = 0; wallcheckh < 4; wallcheckh++) {
			int wallchecky2 = wallcheckv - 2;
			int wallcheckx;
			int wallchecky;
			switch(face) {
			
			case 0:
				wallchecky = playerypos - wallcheckh;
				wallcheckx = playerxpos + wallchecky2;
				wallchecky = wraparoundchecklong(wallchecky);
				wallcheckx = wraparoundcheckshort(wallcheckx);
				wallexist[wallcheckv][wallcheckh] = 0;
				if(gamemap[wallcheckx][wallchecky] == 'W') {
					wallexist[wallcheckv][wallcheckh] = 1;
				};
				if(gamemap[wallcheckx][wallchecky] == '#') {
					wallexist[wallcheckv][wallcheckh] = 2;
				};
			break;
			
			case 1:
				wallcheckx = playerxpos + wallcheckh;
				wallchecky = playerypos + wallchecky2;
				wallcheckx = wraparoundchecklong(wallcheckx);
				wallchecky = wraparoundcheckshort(wallchecky);
				wallexist[wallcheckv][wallcheckh] = 0;
				if(gamemap[wallcheckx][wallchecky] == 'W') {
					wallexist[wallcheckv][wallcheckh] = 1;
				};
				if(gamemap[wallcheckx][wallchecky] == '#') {
					wallexist[wallcheckv][wallcheckh] = 2;
				};
			break;
			
			case 2:
				wallchecky = playerypos + wallcheckh;
				wallcheckx = playerxpos - wallchecky2;
				wallchecky = wraparoundchecklong(wallchecky);
				wallcheckx = wraparoundcheckshort(wallcheckx);
				wallexist[wallcheckv][wallcheckh] = 0;
				if(gamemap[wallcheckx][wallchecky] == 'W') {
					wallexist[wallcheckv][wallcheckh] = 1;
				};
				if(gamemap[wallcheckx][wallchecky] == '#') {
					wallexist[wallcheckv][wallcheckh] = 2;
				};
			break;
			
			case 3:
				wallcheckx = playerxpos - wallcheckh;
				wallchecky = playerypos - wallchecky2;
				wallcheckx = wraparoundchecklong(wallcheckx);
				wallchecky = wraparoundcheckshort(wallchecky);
				wallexist[wallcheckv][wallcheckh] = 0;
				if(gamemap[wallcheckx][wallchecky] == 'W') {
					wallexist[wallcheckv][wallcheckh] = 1;
				};
				if(gamemap[wallcheckx][wallchecky] == '#') {
					wallexist[wallcheckv][wallcheckh] = 2;
				};
			break;
				
			};
		};
	};
	
	
}



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

	for(int snakeai = 0; snakeai < 10; snakeai++) {
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
	if(tempx <= tempy) {
		xovery = tempy / tempx;
		invert = true;
	};
	int pythagorusoutput = (sqrt((pow(tempx, 2)) + (pow(tempy, 2)))) - 1;
	int seconddown = 1;
	if(updown == true) {
		if(invert == false) {
			for(int bruh = 0; bruh < pythagorusoutput; bruh++) {

	
				if(secondary == xovery) {
					seconddown++;
					secondary = 0;
				};
				int frot = northern + seconddown;
				int balls = vres - frot;
				western++;
				
				templateprinter(frot, western, balls, western, layer2, coloursel);
				secondary++;
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
	b[6][7][8][9][0]
	c[1][2][3][4][5]
	d[1][2][p][4][5]
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
	templateprinter(northvert, eastvert, southvert, westvert, 16, 4); //a2
	templateprinter(northvert, westvert, southvert, 0, 15, 4); //a1
	//a3 wall/portal tile
	eastvert = hres - westvert;
	westvert = midx + westvert;
	int trapvert3 = westvert;
	templateprinter(northvert, eastvert, southvert, westvert, 18, 4); //a4
	templateprinter(northvert, hres, southvert, eastvert, 19, 4); //a5
	westvert = midx - plcvert2;
	eastvert = midx + plcvert2;
	templateprinter(northvert, eastvert, southvert, westvert, 17, 4); //a3
	plcvert = midy / 3.5;
	plcvert2 = midx / 2.5;
	westvert = hres / 20;
	northvert = midy - plcvert;
	southvert = midy + plcvert;
	eastvert = midx - plcvert2;
	int plcvert4 = eastvert;
	templateprinter(northvert, eastvert, southvert, westvert, 11, 4); //b2

	printtraptemplate(eastvert, northvert, trapvert2, trapvert1, 11, true, 4);
	templateprinter(northvert, westvert, southvert, 0, 10, 4); //b1
	
	trapvert2 = northvert;
	eastvert = hres - westvert;
	
	westvert = midx + plcvert2;
	int trapvert5 = westvert;
	templateprinter(northvert, eastvert, southvert, westvert, 13, 4); //b4
	printtraptemplate(trapvert3, northvert, westvert, trapvert1, 13, false, 4);
	templateprinter(northvert, hres, southvert, eastvert, 14, 4); //b5
	trapvert1 = eastvert;
	templateprinter(northvert, westvert, southvert, plcvert4, 12, 4); //b3
	
	plcvert = midy / 2;
	plcvert2 = midx / 1.3;
	westvert = 0;
	northvert = midy - plcvert;
	southvert = midy + plcvert;
	eastvert = midx - plcvert2;
	templateprinter(northvert, eastvert, southvert, westvert, 6, 4); // c2
	printtraptemplate(eastvert, northvert, plcvert4, trapvert2, 6, true, 4);
	plcvert3 = eastvert;
	eastvert = hres;
	westvert = midx + plcvert2;
	templateprinter(northvert, eastvert, southvert, westvert, 8, 4); // c4
	printtraptemplate(trapvert5, northvert, westvert, trapvert2, 8, false, 4);
	templateprinter(northvert, westvert, southvert, plcvert3, 7, 4); //c3
	plcvert = midy / 6;
	printtraptemplate(-1, plcvert, plcvert3, northvert, 1, true, 4); // d2
	printtraptemplate(westvert, plcvert, hres, northvert, 3, false, 4); //d4
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



void loadingscreen() {
	system("CLS");
	SetConsoleTextAttribute(hConsole, 7);
	for(int o = 0; 0 < vres; o++) {
		for(int i = 0; i < hres; i++) {
			cout<<" ";
		};
	};
	Sleep(333);
	system("CLS");	
	for(int o = 0; 0 < vres; o++) {
		for(int i = 0; i < hres; i++) {
			cout<<"=";
		};
	};
	Sleep(333);
	system("CLS");	
	for(int o = 0; 0 < vres; o++) {
		for(int i = 0; i < hres; i++) {
			cout<<" ";
		};
	};
	Sleep(333);
	system("CLS");	
	for(int o = 0; 0 < vres; o++) {
		for(int i = 0; i < hres; i++) {
			cout<<"|";
		};
	};
	Sleep(333);
	system("CLS");
	for(int o = 0; 0 < vres; o++) {
		for(int i = 0; i < hres; i++) {
			cout<<" ";
		};
	};
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

void applytemplate(int layer, bool overwrite = false) {
	for(int vertical = 0; vertical < vres; vertical++) {
		for(int horizontal = 0; horizontal < hres; horizontal++) {
				if(templates[horizontal][vertical][layer] != 0) {
				screen[horizontal][vertical] = templates[horizontal][vertical][layer];
				};
				if(overwrite == true) {
				if(templates[horizontal][vertical][layer] != 0) {
				screen[horizontal][vertical] = 5;
				};
			}
			};
		};
	};


void composscreen() {
	int halfvres = vres / 2;
	int halfres = vres - halfvres;
	//resets internal screen to entirely transparent + black
	for(int IntScreenVertical = 0; IntScreenVertical < vres; IntScreenVertical++) {
		for(int IntScreenHorizontal = 0; IntScreenHorizontal< hres; IntScreenHorizontal++) {
			screen[IntScreenHorizontal][IntScreenVertical] = 0;
		};
	};
	for(int IntScreenVertical = halfres; IntScreenVertical < vres; IntScreenVertical++) {
		for(int IntScreenHorizontal = 0; IntScreenHorizontal < hres; IntScreenHorizontal++) {
			screen[IntScreenHorizontal][IntScreenVertical] = 11;
		};
	};
	checkwallexist();
	for(int longside = 3; longside >= 0; longside--) {
		int addtotemplate = 5 * longside;
		int templateselect = addtotemplate + 0;
		if(wallexist[0][longside] == 1) { //p1
			applytemplate(templateselect);
		}else if(wallexist[0][longside] == 2) {
			applytemplate(templateselect, true);
		};
		templateselect = addtotemplate + 4;
		if(wallexist[4][longside] == 1) { //p5
			applytemplate(templateselect);
		} else if(wallexist[4][longside] == 2) {
			applytemplate(templateselect, true);
		};
		templateselect = addtotemplate + 1;
		if(wallexist[1][longside] == 1) { //p2
			applytemplate(templateselect);
		} else if(wallexist[1][longside] == 2) {
			applytemplate(templateselect, true);
		};
		templateselect = addtotemplate + 3;
		if(wallexist[3][longside] == 1) { //p4
			applytemplate(templateselect);
		}else if(wallexist[3][longside] == 2) {
			applytemplate(templateselect, true);
		};
		templateselect = addtotemplate + 2;
		if(wallexist[2][longside] == 1) { //p3
			applytemplate(templateselect);
		} else if(wallexist[2][longside] == 2) {
			applytemplate(templateselect, true);
		};
	
	};
}
	




void renderscreen() {
		/*while(gamerun) {
		while(runtime == prevrun) {
				this_thread::sleep_for(chrono::milliseconds(10));
		};
		*/
		system("CLS");//clears screen
		prevrun = runtime;
	for(int breen = 0; breen <= vres; breen++) { //renders the entire screen according to the resolution 
		for(int brug = 0; brug <= hres; brug++) {
			SetConsoleTextAttribute(hConsole, screen[brug][breen]);
			cout<<"#";
		};
		cout<<endl;
		};

	//};
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

void turnleft() {
	face--;
	if(face < 0) {
		face = 3;
	};
}

void turnright() {
	face++;
	if(face > 3) {
		face = 0;
	};
}
	
void tryforward() {
	int tempplayerx = playerxpos;
	int tempplayery = playerypos;
	switch(face) {
		case 0:
		tempplayery--;
		break;
		
		case 1:
		tempplayerx++;
		break;
		
		case 2:
		tempplayery++;
		break;
		
		case 3:
		tempplayerx--;
		break;
	};
	if(tempplayerx == 3) {
		tempplayerx = 45;
	} else if(tempplayerx == 46) {
		tempplayerx = 4;
	};
	if(tempplayery == 3) {
		tempplayery = 45;
	} else if(tempplayery == 46) {
		tempplayery = 4;
	};
	
	if(gamemap[tempplayerx][tempplayery] != 'W') {
	playerxpos = tempplayerx;
	playerypos = tempplayery;
	};
	if(gamemap[playerxpos][playerypos] == '#') {
		generatemap(playerxpos, playerypos);
		Sleep(30);
		loadingscreen();
	};
	
	
}

void showmap() {
	system("CLS");
	for(int james = 0; james < 50; james++) {
		for( int bob = 0; bob < 50; bob++) {
			SetConsoleTextAttribute(hConsole, 4);
			if(bob == 3||bob == 46||james == 3||james == 46) {
				SetConsoleTextAttribute(hConsole, 5);
			};
			if(gamemap[bob][james] != 'W') {
				SetConsoleTextAttribute(hConsole, 3);
			};
			if(bob == playerxpos&&james == playerypos) {
				SetConsoleTextAttribute(hConsole, 9);
				cout<<"p";
			} else {
			cout<<gamemap[bob][james];
			};
		};
		cout<<endl;
	};
	cout<<"portal is at "<<mapx<<", "<<mapy<<endl;
};


int main() {
    char input;
vres = 40;
hres = 156;
    setres();
	auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Type any key. Press 'q' to quit." << std::endl;
	system("CLS");
	srand(time(NULL));
	
	settempplates();
    generatemap(25, 25);
	playerxpos = 25;
	playerypos = 25;
	
	
	

	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	
	cout<<"press any button to continue"<<endl;
	
	system("PAUSE");
    while (true) {
        if (_kbhit()) {  // Check if a key has been pressed
            input = _getch();  // Read the key that was pressed
            if(input == 'w') {
				tryforward();
			};
			if(input == 'a') {
				turnleft();
			};
			if(input == 'd') {
				turnright();
			};
            if (input == 'q')
                break;  // Exit the loop if 'q' is pressed
            setCursorPosition(0, 0);
			
		
				composscreen();
				renderscreen();
				SetConsoleTextAttribute(hConsole, 15);
			if(input == 'm') {
			showmap();
			};
				cout<<"facing: "<<playerfacing[face]<<"   x = "<<playerxpos<<"   y = "<<playerypos;
		};
			
        }
		
        
        // Perform other tasks or update the program state here
    }











//raptley if you are reading this i love you <3