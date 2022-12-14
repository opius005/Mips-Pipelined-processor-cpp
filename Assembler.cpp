#include <bits/stdc++.h>
using namespace std;

int linecounter = 0;
vector<string> symbols;
vector<string> line;
vector<string> label;
vector<string> insmem;

string regnum(string str){
	string temp;
	if(str == "$0") temp = "00000";
        else if(str == "$1") temp = "00001";
        else if(str == "$2") temp = "00010";
        else if(str == "$3") temp = "00011";
        else if(str == "$4") temp = "00100";
        else if(str == "$5") temp = "00101";
        else if(str == "$6") temp = "00110";
        else if(str == "$7") temp = "00111";
        else if(str == "$8") temp = "01000";
        else if(str == "$9") temp = "01001";
        else if(str == "$10") temp = "01010";
        else if(str == "$11") temp = "01011";
        else if(str == "$12") temp = "01100";
        else if(str == "$13") temp = "01101";
        else if(str == "$14") temp = "01110";
        else if(str == "$15") temp = "01111";
        else if(str == "$16") temp = "10000";
        else if(str == "$17") temp = "10001";
        else if(str == "$18") temp = "10010";
        else if(str == "$19") temp = "10011";
        else if(str == "$20") temp = "10100";
        else if(str == "$21") temp = "10101";
        else if(str == "$22") temp = "10110";
        else if(str == "$23") temp = "10111";
        else if(str == "$24") temp = "11000";
        else if(str == "$25") temp = "11001";
        else if(str == "$26") temp = "11010";
        else if(str == "$27") temp = "11011";
        else if(str == "$28") temp = "11100";
        else if(str == "$29") temp = "11101";
        else if(str == "$30") temp = "11110";
        else if(str == "$31") temp = "11111";
        return temp;
}

void labels(string str){
	int i = 0;
	string temp = "";
	while(islower(str[i])){
		temp += str[i];
		i++;
	}
	if((temp != "add") && (temp != "addi") && (temp != "mul") && (temp != "or") && (temp != "xor") && (temp != "and") && (temp != "j") && (temp != "beq") && (temp != "bne") && (temp != "lw") && (temp != "sw") && (temp != "sub"))
	label.push_back(temp);
}

int address(string str){
	int labelno;
	for(int i = 0;i < label.size();i++){
		if(str == label[i])
			labelno = i;
	}
	if(str == "end")
		return symbols.size()-label.size();
	for(int j = 0; j < symbols.size(); j++){
		if(str == symbols[j]){
			return (j - labelno);
		}
	}
	return 0;
}

void insertsymbols(string str){
	int i = 0;
	string temp = "";
	while(islower(str[i])){
		temp += str[i];
		i++;
	}
	symbols.push_back(temp);
}


void ins(string str){
	int i = 0;
	string temp = "";
	while(islower(str[i])){
		temp += str[i];
		i++;
	}
	i--;

	if((temp=="add" )|| (temp == "or") || (temp == "xor") || (temp == "and") || (temp == "sub")){
		string finalbin = "000000";
		string r;
		string bin ="";
		string newreg = "";
		for(;i < str.length();i++){
			if(str[i] == '$'){
				newreg = "$";
				while(isdigit(str[i+1]) && (i+1) < str.length()){
					newreg += str[i+1];
					i++;
				}
				r = regnum(newreg);
				bin+= r;
			}
		}
		string rs,rt,rd;
		rd = bin.substr(0,5);
		rt = bin.substr(5,5);
		rs = bin.substr(10,5);
		if(temp == "add")
			finalbin = finalbin + rs + rt + rd +"00000100000";
		else if(temp == "or")
			finalbin = finalbin + rs + rt + rd +"00000100101";
		else if(temp == "xor")
			finalbin = finalbin + rs + rt + rd +"00000100110";
		else if(temp == "and")
			finalbin = finalbin + rs + rt + rd +"00000100100";
		else if(temp == "sub")
			finalbin = finalbin + rs + rt + rd +"00000100010";
		insmem.push_back(finalbin);
	}
	else if(temp == "mul"){
		string finalbin = "011100";
		string r;
		string bin ="";
		string newreg = "";
		for(;i < str.length();i++){
			if(str[i] == '$'){
				newreg = "$";
				while(isdigit(str[i+1]) && (i+1) < str.length()){
					newreg += str[i+1];
					i++;
				}
				r = regnum(newreg);
				bin+= r;
			}
		}
		string rs,rt,rd;
		rd = bin.substr(0,5);
		rt = bin.substr(5,5);
		rs = bin.substr(10,5);
		finalbin = finalbin + rs + rt + rd + "00000000010";
		insmem.push_back(finalbin);
	}
	
	else if(temp == "addi"){
		string finalbin = "001000";
		string r;
		string bin;
		string newreg = "";
		int j = i+5;
		for(;i < str.length();i++){
			if(str[i]== '$'){
				newreg = "$";
				while(isdigit(str[i+1]) && (i+1) < str.length()){
					newreg += str[i+1];
					i++;
					j++;
				}
				r = regnum(newreg);
				bin += r;
			}
		}
		string rs,rt;
		rt = bin.substr(0,5);
		rs = bin.substr(5,5);
		string constant = "";
			if(str[j] = ','){
				while(isdigit(str[j+1])){
					constant += str[j+1];
					j++;
				}
				stringstream ss;
				int cons;
				ss<<constant;
				ss >> cons;
				finalbin = finalbin + rs + rt + (bitset<16>(cons)).to_string();
				insmem.push_back(finalbin);
			}
	}
	
	else if(temp == "j"){
		string finalbin = "000010";
		i = i + 2;
		string lbl = "";
		while(islower(str[i])){
			lbl+=str[i];
			i++;
		}
		int num = address(lbl);
		finalbin += (bitset<26>(num)).to_string();
		insmem.push_back(finalbin);
	}
	else if(temp == "beq" || temp == "bne"){
		string finalbin;
		if(temp == "beq")
			finalbin = "000100";
		else if(temp == "bne")
			finalbin = "000101";
		string bin = "";
		string r;
		string newreg = "";
		int j = i+5;
		for(;i < str.length();i++){
			if(str[i] == '$'){
				newreg = "$";
				while(isdigit(str[i+1]) && (i+1) < str.length()){
					newreg += str[i+1];
					i++;
					j++;
				}
				r = regnum(newreg);
				bin += r;
			}
		}
		string rt = bin.substr(0,5);
		string rs = bin.substr(5,5);
		string lbl = "";
		while(islower(str[j+1]) && (j+1) < str.length()){
			lbl+=str[j+1];
			j++;
		}
		int num = address(lbl);
		finalbin = finalbin + rs + rt + (bitset<16>(num)).to_string();
		insmem.push_back(finalbin);
	}
	
	else if(temp == "lw" || temp == "sw"){
		string finalbin;
		if(temp == "lw")
			finalbin = "100011";
		else if(temp == "sw")
			finalbin = "101011";
		string bin = "";
		string r;
		string newreg = "";
		string constant = "";
		for(;i < str.length();i++){
			if(str[i] == '$'){
				newreg = "$";
				while(isdigit(str[i+1]) && (i+1) < str.length()){
					newreg += str[i+1];
					i++;
				}
				r = regnum(newreg);
				bin += r;
			}
			if(str[i] == ','){
				while(isdigit(str[i+1])){
					constant += str[i+1];
					i++;
				}
				stringstream ss;
				int cons;
				ss<<constant;
				ss >> cons;
				constant = (bitset<16>(cons)).to_string();
			}
		}
		string rt = bin.substr(0,5);
		string rs = bin.substr(5,5);
		finalbin = finalbin + rs + rt + constant ;
		insmem.push_back(finalbin);
	}
}



int main(){
	string newstr;
	ifstream assembly_code("AssemblyInput.txt");
	if(assembly_code.is_open()){
		while(getline(assembly_code,newstr)){
			if(newstr.size() > 0){
				line.push_back(newstr);
			}
		}
	}
	assembly_code.close();
	for(int i = 0;i < line.size();i++){
		insertsymbols(line[i]);
	}
	for(int i = 0;i < line.size();i++){
		labels(line[i]);
	}
	for(int i = 0;i < line.size();i++){
		ins(line[i]);
	}
	insmem.push_back("11111111111111111111111111111111");
	vector<string> machine_code;
	int y = 0;
	int z = 4;
	for(int i = 0;i < insmem.size();i++){
		while(z){
			machine_code.push_back(insmem[i].substr(y,8));
			z--;
			y = y+8;
		}
		y = 0;
		z = 4;
	}
	ofstream instruction_memory("instruction_memory.txt");
	for(int i = 0;i < machine_code.size();i++){
		instruction_memory << machine_code[i] << endl;
	}
	instruction_memory.close();
}