//Name: Jonathan Quigley
//Worked with: Josh Sproul
void printString(char*);
void printChar(char);
void readString(char*);
void readSector();
void readFile();
void executeProgram();
void terminate();
void handleInterrupt21();

void main(){

	makeInterrupt21();
	interrupt(0x21, 4, "shell", 0, 0);
	while(1);

}

void printString(char* chars){

	int c = 0;
	while(chars[c] != 0x0){
		printChar(chars[c]);
		c++;
	}

}

void printChar(char c){

	c= interrupt(0x10, 0xe*256+c, 0, 0, 0);

}

void readString(char* line){

	char c;
	int n = 0;
	while(1){
		c = interrupt(0x16, 0, 0, 0, 0);
		if(c == 0xd){
			break;
		}
		if(c == 0x8){
			line[n--];
			printChar(0x8);
			printChar(' ');
			printChar(0x8);
		}
		else{
			line[n++] = c;
			printChar(c);
		}
	}
	line[n++] = 0xd;
	line[n++] = 0xa;
	line[n++] = 0x0;
	printChar(0xd);
	printChar(0xa);

}

void readSector(char* buffer, int sector){
/*
	int AH = 2;
	int AL = 1;
	int CH = 0;
	int CL = (sector +1);
	int DH = 0;
	int DL = 0x80;
*/
//optimized code with less integers (AL, AH, CL, CH, etc.)
	int AX = 2*256 + 1;
	int BX = buffer;
	int CX = 0*256 + (sector+1);
	int DX = 0*256 + (0x80);
	interrupt(0x13, AX, BX, CX, DX);

}

void readFile(char* name, char* buffer, int* sector){
	int e = 0;
	int i = 0;
	int j = 0;
	char dir[512];
	readSector(dir, 2);
	*sector = 0;
	for(e = 0; e < 512; e=e+32){
		for(i = 0; i < 6; i++){
			if(name[i] != dir[e + i]){
				break;
			}
			else if(name[i] == 0 && dir[e + i] == 0|| i ==5){
				while(dir[e + 6 + j] != 0){
					readSector(buffer, dir[e + 6 + j]);
					buffer = buffer + 512;
					*sector = *sector + 1;
					j++;
				}
				return;
			}
		}
	}
}

void executeProgram(char* name){
	int x;
	char buffer[13312];
	int seg = 0x2000;
	int sectors;
	readFile(name, buffer, &sectors);
	if(sectors > 0){
		for(x = 0; x < 13312; x++){
			putInMemory(seg, x, buffer[x]);
		}
	}
	else{
		printString("No file found");
	}
	launchProgram(seg);

}

void terminate(){
	char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';
	executeProgram(shellname);
	while(1);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){

	//printString("Hello world");
	if(ax == 0){
		printString(bx);
	}
	else if(ax == 1){
		readString(bx);
	}
	else if(ax == 2){
		readSector(bx, cx);
	}
	else if(ax == 3){
		readFile(bx, cx, dx);
	}
	else if(ax == 4){
		executeProgram(bx);
	}
	else if(ax == 5){
		terminate();
	}
	else{
		printString("Out of Bounds exception for interrupt");
	}

}









