#include <iostream>
#include <conio.h>  // Include the conio.h header for _getch() function on Windows
#include <windows.h>
#include <thread>


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
using namespace std; //just so i dont need to write std every fucking time
int hres; //horizontal resolution
int vres; //vertical resolution


int playerxpos;
int playerypos;

bool gamerun = true;

char map[5][5][2] = {{
	{'w','w','e','w','w'},
	{'w','w','e','w','w'},
	{'e','e','e','e','e'},
	{'w','w','e','w','w'},
	{'w','w','w','w','w'}},
	{{' ',' ','a',' ',' '},
	{' ',' ',' ',' ',' '},
	{' ',' ',' ',' ','m'},
	{' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' '}}
	
};
bool render1;

void engine() {
	
	thread clock(gameclock);
	thread externalscr(renderscreen);
	thread internalscr(composscreen);
};



void gameclock() { //runs internal clock to sync everything to
	bool clockrun = true;
	while(clockrun) {
		render1 = true;
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
	


bool rendering = true;

void renderscreen() {
		while(gamerun) {
		while(!render1) {
				sleep(10);
		};
		
		system("CLS");//clears screen
		render1 = false
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
    
    return 0;
}