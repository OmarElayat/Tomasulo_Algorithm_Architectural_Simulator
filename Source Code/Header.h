#pragma once
#include <iostream>
#include <map>
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include<algorithm>
#include <cstdlib>
#include<sstream>
#include<format>
#include<set>
#include <bitset>
#include<queue>
using namespace std;


enum instruction_type {
	LOAD,
	STORE,
	JMP,
	BEQ,
	JAL,
	RET,
	ADD,
	SUB,
	ADDI,
	NAND,
	MUL
};


enum Station_Name {
	LOAD_st,
	STPRE_st,
	UJ_st,
	BEQ_st,
	R_st,
	NAND_st,
	MUL_st
};


struct instructions {
	instruction_type inst_type;
	Station_Name station; 
	int rd;
	int imm;
	int rs1;
	int rs2;
	bool issue_flag = 0;
	bool execute_flag = 0;
	bool wb_flag = 0;
	bool commit_flag = 0;
	int execute_clock_left;
	bool execute_started=false;
	int rob_index;
	int rs_index;
	int issued_cycle;
	int finished_execution_cycle;
	int started_execution_cycle;
	int written_cycle;
	int commit_cycle;
};

instructions* instruction_list;

bool common_data_bus_1_busy = false;
bool common_data_bus_2_busy = false;

bool commit_1_busy = false;
bool commit_2_busy = false;

bool branch = false;

struct rob {
	bool ready = false;
	instruction_type inst;
	int Dest;
	int value;
	bool empty = true;
};

rob reorder_buffer[9];
int rob_head = 1, rob_tail = 1;


struct reservation_station {
	Station_Name st_name;
	bool busy = false;
	string operation;
	int Vj;
	int Vk;
	int Qj =-1;
	int Qk= -1;
	int Dest;
	int address;
};
reservation_station load_stations[2];
reservation_station STPRE_stations[2];
reservation_station UJ_stations[2];
reservation_station BEQ_stations[2];
reservation_station R_stations[3];
reservation_station NAND_stations[2];
reservation_station MUL_stations[2];
reservation_station stations[15];

int Registers[8];
int Register_Status[8];

struct instruction_status {
	string inst; 
	int issue;
	int Execute;
	int Written;
	int Comitted;
};



int mapping(string name) {
	// The function is used to map the Registers to their index number
	// Parameters :
	//      name: name of the register
	//	Out : the number of the register in Registers array

	map<string, int> mymap;

	mymap["R0"] = 0;
	mymap["R1"] = 1;
	mymap["R2"] = 2;
	mymap["R3"] = 3;
	mymap["R4"] = 4;
	mymap["R5"] = 5;
	mymap["R6"] = 6;
	mymap["R7"] = 7;
	return mymap.at(name);

}
string Reg_num[8] = { "R0", "R1","R2","R3","R4","R5","R6","R7"};
string func_name[11] = { "Load", "STORE","JMP","BEQ","JAL","RET","ADD","SUB","ADDI","NAND","MUL"};
double branch_miss;
double branch_hit;
instruction_type mapping_inst(string name) {
	// The function is used to map the Registers to their index number
	// Parameters :
	//      name: name of the register
	//	Out : the number of the register in Registers array

	map<string, instruction_type> mymap;

	mymap["LOAD"] = LOAD;
	mymap["STORE"] = STORE;
	mymap["JMP"] = JMP;
	mymap["BEQ"] = BEQ;
	mymap["JAL"] = JAL;
	mymap["RET"] = RET;
	mymap["ADD"] = ADD;
	mymap["SUB"] = SUB;
	mymap["ADDI"] = ADDI;
	mymap["NAND"] = NAND;
	mymap["MUL"] = MUL;



	return mymap.at(name);
}