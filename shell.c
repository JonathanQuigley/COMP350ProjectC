//Jonathan Quigley
//Worked with: Josh Sproul

void type();
void exec();

void main(){

	int i = 0;
	int j = 0;
	int x = 0;
	char input[100];
	char cmd[4];
	char fileName[96];
	syscall(0, "SHELL>:");
	syscall(1, input);
	for(i = 0; i < 5; i++){
		cmd[i] = input[i];
	}
	for(j = 5; j < 100; j++){
		if(input[j] != '\0'){
			fileName[x] = input[j];
			x++;
		}
	}
	if(cmd[0] == 't' && cmd[1] == 'y' && cmd[2] == 'p' && cmd[3] == 'e'){
		type(fileName);
	}
	else if(cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] =='e' && cmd[3] == 'c'){
		exec(fileName);
	}
	else{
		syscall(0, "No such command\r\n");
	}

}


void type(char* name){

	char buffer[13312];
	int sectorsRead;
	syscall(3, name, buffer, &sectorsRead);
	if(sectorsRead > 0){
		syscall(0, buffer);
	}
	else{
		syscall(0, "File Not Found");
	}
	syscall(0, "\r\n");

}

void exec(char* name){

	syscall(4, name);
	syscall(5);

}

