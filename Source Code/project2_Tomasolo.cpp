// project2_Tomasolo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Header.h"

class Tom_assembler{
public:
int count = 0;                              // Number of instructions in File
string* lines;							  // Instruction file lines
int pc = 0;								  // program counter
map<string, int> looping_inst_map;		  // map for instruction labels with their address
map<int, int> MemoryIn_map;                // map of the memory used with their value

queue<instructions> load_store_q;

void read_file(string filename) {
	// This function creates an array of strings and each string represents an instruction with its index 
	// Parameters :
	//			filename: name of instruction file
	//	Out : None
	string line;
	 count = 0;
	ifstream myfile(filename);
	ifstream myfile2(filename);
	if (myfile.is_open())
	{

		while (getline(myfile, line)) {
			if (line[line.length() - 1] != ':') {
				count++;
			}
		}
		myfile.close();
	}

	if (myfile2.is_open()) {
		lines = new string[count];
		int i = 0;
		while (getline(myfile2, line)) {

			if (line[line.length() - 1] == ':') {
				string label = line.substr(0, line.find(':'));

				//cout << label << endl;

				//string label = line.substr(0, line.length() - 1);
				string ii = to_string(i);
				looping_inst_map.insert({ label,i });   //need to push in a vector with a label 
				looping_inst_map.insert({ ii,i });

			}
			else {
				lines[i] = line;
				i++;

			}
		}
		myfile2.close();
		cout << "file readed successfully..." << endl;
		instruction_list = new instructions[count];
	}
	else cout << "Unable to open instrucion file";
}


void read_memory(string memoryfile) {
	// This function reads the content of the memory file and save it  
	// Parameters :
	//			filename: name of memory file
	//	Out : None
	string linemem;
	ifstream myfilemem(memoryfile);
	if (myfilemem.is_open())
	{

		while (getline(myfilemem, linemem)) {
			string address = linemem.substr(0, linemem.find(' '));
			string value = linemem.substr(linemem.find(' ') + 1, linemem.find('\n'));
			int address2 = stoi(address);
			int value2 = stoi(value);
			MemoryIn_map.insert({ address2,value2 });
			cout << address << endl << value << endl;
		}
		cout << " memory file" << endl;
		myfilemem.close();
	}
	else { cout << "cannot open memory file" << endl; }
}

void parse(int pci) {
	// This function defines command type from instructions and call compute with operands 
	// Parameters :
	//			pci: number of the current program counter to count on
	//	Out : None
	string line;
	for (pc = pci; pc < count; pc++)
	{
		line = lines[pc];
		cout << pc << endl;
		if (line[line.length() - 1] != ':') {
			string deli = " ";
			string instruct = line.substr(0, line.find(deli));
			string operands = line.substr(line.find(deli) + 1, line.length());
			cout << operands << endl << instruct << endl;

				cout << "pc before ex" << pc << endl;
				compute(instruct, operands , pc);

			if (Registers[0]) {
				Registers[0] = 0;
			}
			if (pc == count) { break; }
			//write_mem();
		}
	}
}
void compute(string ins_type, string operands , int index) {
	if (ins_type == "ADD" || ins_type == "SUB" ||  ins_type == "NAND" || ins_type == "MUL") {
		string rd1 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rd = mapping(rd1);
		string rs11 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rs1 = mapping(rs11);
		string rs21 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rs2 = mapping(rs21);
		cout << "rs1: " << rs1 << "  " << "rs2: " << rs2 << "  " << "rd: " << rd << endl;

		instruction_list[index].rd = rd;
		instruction_list[index].rs1 = rs1;
		instruction_list[index].rs2 = rs2;
		instruction_list[index].inst_type = mapping_inst(ins_type);
		if (ins_type == "NAND") {
			instruction_list[index].station = NAND_st;
		}
		else if (ins_type == "MUL") {
			instruction_list[index].station = MUL_st;
		}
		else {
			instruction_list[index].station = R_st;
		}
	}
	else if (ins_type == "ADDI") {

		string rd1 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rd = mapping(rd1);

		string rs11 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rs1 = mapping(rs11);

		string immedstring = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int immediate = stoi(immedstring);
		cout << "rs1: " << rs1 << "  " << "imm: " << immediate << "  " << "rd: " << rd << endl;

		instruction_list[index].rd = rd;
		instruction_list[index].rs1 = rs1;
		instruction_list[index].imm = immediate;
		instruction_list[index].inst_type = mapping_inst(ins_type);
		instruction_list[index].station = R_st;
	}
	else if (ins_type == "LOAD") {

		string rs11 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rs1 = mapping(rs11);

		string immedstring = operands.substr(0, operands.find("("));
		operands = operands.substr(operands.find("(") + 1, operands.length());
		int immediate = stoi(immedstring);

		string rs21 = operands.substr(0, operands.find(")"));
		operands = operands.substr(operands.find(")") + 1, operands.length());
		int rs2 = mapping(rs21);
		cout << "rs1: " << rs1 << "  " << "imm: " << immediate << "  " << "rs2: " << rs2 << endl;

		instruction_list[index].imm = immediate;
		instruction_list[index].rd = rs1;
		instruction_list[index].rs1 = rs2;
		instruction_list[index].inst_type = mapping_inst(ins_type);
		instruction_list[index].station = LOAD_st;
	//	load_store_q.push(instruction_list[index]);
	}
	else if (ins_type == "STORE") {

		string rs11 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rs1 = mapping(rs11);

		string immedstring = operands.substr(0, operands.find("("));
		operands = operands.substr(operands.find("(") + 1, operands.length());
		int immediate = stoi(immedstring);

		string rs21 = operands.substr(0, operands.find(")"));
		operands = operands.substr(operands.find(")") + 1, operands.length());
		int rs2 = mapping(rs21);
		cout << "rs1: " << rs1 << "  " << "imm: " << immediate << "  " << "rs2: " << rs2 << endl;

		instruction_list[index].imm = immediate;
		instruction_list[index].rs2 = rs1;
		instruction_list[index].rs1 = rs2;
		instruction_list[index].inst_type = mapping_inst(ins_type);
		instruction_list[index].station = STPRE_st;
	//	load_store_q.push(instruction_list[index]);
	}
	else if (ins_type == "BEQ") {

		string rs11 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rs1 = mapping(rs11);
		string rs21 = operands.substr(0, operands.find(","));
		operands = operands.substr(operands.find(",") + 1, operands.length());
		int rs2 = mapping(rs21);

		string imm_address = operands.substr(0, operands.length());
		operands = operands.substr(operands.find(",") + 1, operands.length());
		//int imm_address_hexa = Address_mapping(imm_address);

		cout << "rs1: " << rs1 << "  " << "rs2: " << rs2 << "  " << "imm: " << imm_address << endl;
		cout << "values:  " << Registers[rs1] << "    " << Registers[rs2] << endl;

		instruction_list[index].imm = looping_inst_map[imm_address];
		instruction_list[index].rs1 = rs1;
		instruction_list[index].rs2 = rs2;
		instruction_list[index].inst_type = mapping_inst(ins_type);
		instruction_list[index].station = BEQ_st;
	}
	else if (ins_type == "JAL" || ins_type == "JMP") {

		//string rd1 = operands.substr(0, operands.find(","));
		//operands = operands.substr(operands.find(",") + 1, operands.length());
		//int rd = mapping(rd1);

		string imm_address_label = operands.substr(0, operands.length());
		//operands = operands.substr(operands.find(",") + 1, operands.length());

		//cout << "rd: " << rd << "  " << "imm: " << imm_address_label << endl;

		instruction_list[index].imm = looping_inst_map[imm_address_label];
		instruction_list[index].inst_type = mapping_inst(ins_type);
		instruction_list[index].station = UJ_st;
	}
	else if (ins_type == "RET") {
		instruction_list[index].inst_type = mapping_inst(ins_type);
		instruction_list[index].station = UJ_st;
	}
	else {
		cout << "INVALID INSTRUCTION" << endl;
	}
}
// assuming the instructions are parsed and saved in a date structure
void instruction_list_filler() {

}
int clock_count = 0;
void process() {   /*    unfinished     */
	
	int next_issue=0;
	int next_branch_issue = 0;

	instruction_list[0].issue_flag = true;
	//instruction_list[1].issue_flag = true;

	while (clock_count <165) {
		
		clock_count++;
		cout << "clock number:  " << clock_count << endl;
		//starting condition
		common_data_bus_1_busy = false;
		common_data_bus_2_busy = false;

		commit_1_busy = false;
		commit_2_busy = false;

		for(int i =0; i<count;i++){
			if (!branch) {
				if (instruction_list[i].commit_flag) next_branch_issue = commit(instruction_list, i);
				if (instruction_list[i].wb_flag) wb(instruction_list, i);
				if (instruction_list[i].execute_flag) execute(instruction_list, i);
				if (instruction_list[i].issue_flag) next_issue = issue(instruction_list, i, next_issue);
			}
		}
		if (branch) {
			for (int i = 0; i < count; i++) {
				instruction_list[i].commit_flag = false;
				instruction_list[i].execute_flag = false;
				instruction_list[i].wb_flag = false;
				instruction_list[i].issue_flag = false;
				instruction_list[i].execute_started = false;
				instruction_list[i].rob_index = 0;
				instruction_list[i].rs_index = 0;
			}
			instruction_list[next_branch_issue].issue_flag = true;
			//instruction_list[next_branch_issue + 1].issue_flag = true;
			cout << "next branch issue instruction" << next_branch_issue << endl;
			branch = false;
		}
		else {
			instruction_list[next_issue].issue_flag = true;
		//	instruction_list[next_issue + 1].issue_flag = true;
		}

		//dual issue
		for (int i = 0; i < count; i++) {
				if (instruction_list[i].issue_flag) next_issue = issue(instruction_list, i, next_issue);
		}
		instruction_list[next_issue].issue_flag = true;
		//_______________________________________________
		


		//if (clock_count == 6 /*commit flag of last instruction raised*/) {
		///	break;
		//}
	}

}
int issue(instructions* inst_list, int index, int next_issue) {

	int empty_index;
	int h;


	if ( check_empty_station(inst_list[index].station,empty_index) && !is_rob_full()) {
		cout << "empty_index"<<empty_index << endl;
		cout << "iss_flag " << inst_list[index].issue_flag << endl;
		reorder_buffer[rob_tail].inst = inst_list[index].inst_type;
		reorder_buffer[rob_tail].ready = false;
		reorder_buffer[rob_tail].empty = false;
		stations[empty_index].busy = true;
		stations[empty_index].Dest = rob_tail;

		inst_list[index].rs_index = empty_index;

		// rs1
		if (!(inst_list[index].inst_type == JAL || inst_list[index].inst_type == JMP || inst_list[index].inst_type == RET)) {
			if (Register_Status[inst_list[index].rs1]) {
				h = Register_Status[inst_list[index].rs1];
				if (!reorder_buffer[h].empty) {
					if (reorder_buffer[h].ready) {
						stations[empty_index].Vj = reorder_buffer[h].value;
						stations[empty_index].Qj = -1;
					}
					else {
						stations[empty_index].Qj = h;
					}
				}
			}
			else {
				stations[empty_index].Vj = Registers[inst_list[index].rs1];
				stations[empty_index].Qj = -1;
			}
		}


		// rs2
		if ((inst_list[index].inst_type == STORE || inst_list[index].inst_type == BEQ || inst_list[index].inst_type == ADD
			|| inst_list[index].inst_type == SUB || inst_list[index].inst_type == NAND || inst_list[index].inst_type == MUL)) {
			if (Register_Status[inst_list[index].rs2]) {
				h = Register_Status[inst_list[index].rs2];
				if (!reorder_buffer[h].empty) {
					if (reorder_buffer[h].ready) {
						stations[empty_index].Vk = reorder_buffer[h].value;
						stations[empty_index].Qk = -1;
					}
					else {
						stations[empty_index].Qk = h;
					}
				}
			}
			else {
				stations[empty_index].Vk = Registers[inst_list[index].rs2];
				stations[empty_index].Qk = -1;
			}
		}
		//rd
		if (inst_list[index].station == R_st || inst_list[index].station == NAND_st || inst_list[index].station == MUL_st || inst_list[index].station == LOAD_st) {
			if (inst_list[index].rd != 0) {
				reorder_buffer[rob_tail].Dest = inst_list[index].rd;
				Register_Status[inst_list[index].rd] = rob_tail;
			}
		}

		if (inst_list[index].inst_type == JAL) {
			reorder_buffer[rob_tail].Dest = 1;
			Register_Status[1] = rob_tail;
		}

		//queue
		if (inst_list[index].inst_type == LOAD || inst_list[index].inst_type == STORE) {
			load_store_q.push(instruction_list[index]);
		}

		//imm
		if (inst_list[index].inst_type == LOAD || inst_list[index].inst_type == STORE || inst_list[index].inst_type == JMP
			|| inst_list[index].inst_type == BEQ || inst_list[index].inst_type == JAL) {
			stations[empty_index].address = inst_list[index].imm;
		}
		else if (inst_list[index].inst_type == ADDI) {
			stations[empty_index].Vk = inst_list[index].imm;
		}
		else if (inst_list[index].inst_type == RET) {
			if (Register_Status[1]) {
				h = Register_Status[1];
				if (reorder_buffer[h].ready) {
					stations[empty_index].Vj = reorder_buffer[h].value;
					stations[empty_index].Qj = -1;
				}
				else {
					stations[empty_index].Qj = h;
				}
			}
			else {
				stations[empty_index].Vj = Registers[1];
				stations[empty_index].Qj = -1;
			}
		}
		inst_list[index].rob_index = rob_tail;

		// incrementing reorder buffer pointer
		if (rob_tail != 8) {
			rob_tail++;
		}
		else {
			rob_tail = 1;
		}

		stations[empty_index].operation = func_name[inst_list[index].inst_type];



		// branch predictor
		if (inst_list[index].inst_type == BEQ) {
			cout << "immediate = " << inst_list[index].imm << endl;
			if (inst_list[index].imm < index ) {
				inst_list[index].execute_flag = true;
				next_issue = inst_list[index].imm + 1;
				inst_list[index].issue_flag = false;
				cout << "issued inst " << index << endl;
				cout << "branch predicted ttaken" << endl;
				cout << "immediate = " << inst_list[index].imm << endl;
				inst_list[index].issued_cycle = clock_count;
				return next_issue;
			}
		}

/*		if (inst_list[index].inst_type == JAL || inst_list[index].inst_type == JMP) {
			inst_list[index].execute_flag = true;
			next_issue = inst_list[index].imm + 1;
			inst_list[index].issue_flag = false;
			cout << "issued inst " << index << endl;
			cout << "jump taken" << endl;
			cout << "immediate = " << inst_list[index].imm << endl;
			inst_list[index].issued_cycle = clock_count;
			return next_issue;
		}
		*/


		//printing
		inst_list[index].issued_cycle = clock_count;
		// flags 
		inst_list[index].execute_flag = true;
		next_issue = index + 1;
		inst_list[index].issue_flag = false;
		cout << "issued inst " << index << endl;
		return next_issue;

	}
	else {
		cout << "instruction" << index << "not issued this cycle" << endl;
		return next_issue;
	}
	
}
void execute(instructions* inst_list, int index) {
	//fp operations
	int i = inst_list[index].rs_index;


	// decrease cycles remaining & finish execuion
	if (inst_list[index].execute_started) {
		if (inst_list[index].execute_clock_left > 0) {
			inst_list[index].execute_clock_left--;
		}
		if (inst_list[index].execute_clock_left == 0) {
			cout << "instruction " << index << "executed" << endl;
			inst_list[index].finished_execution_cycle = clock_count;
			inst_list[index].execute_flag = false;
			inst_list[index].wb_flag = true;
			if (inst_list[index].inst_type == STORE || inst_list[index].inst_type == LOAD) {
				load_store_q.pop();
			}
		}

	}

	if (!inst_list[index].execute_started && (inst_list[index].inst_type == ADDI || inst_list[index].inst_type == ADD || inst_list[index].inst_type == MUL || inst_list[index].inst_type == SUB || inst_list[index].inst_type == NAND))
	{
		if (stations[i].Qj == -1 && stations[i].Qk == -1) {
			if (inst_list[index].inst_type == NAND) {
				inst_list[index].execute_clock_left = 1;
			}
			else if (inst_list[index].inst_type == MUL) {
				inst_list[index].execute_clock_left = 10;
			}
			else {
				inst_list[index].execute_clock_left = 2;
			}
				inst_list[index].execute_started = true;
				inst_list[index].started_execution_cycle = clock_count;
		}
	}
	//LOAD
	if ((!inst_list[index].execute_started) && (inst_list[index].inst_type == LOAD)) {
		if (stations[i].Qj == -1 && load_store_q.front().inst_type != STORE) {
			stations[i].address = stations[i].Vj + stations[i].address;
			if (!ROB_store_address(inst_list[index].rob_index, stations[i].address)) {
				inst_list[index].execute_clock_left = 3;
				inst_list[index].execute_started = true;
				inst_list[index].started_execution_cycle = clock_count;
				// please put in wb
				//inst_list[index].rd = MemoryIn_map[stations[i].address];
			}
		}
	}
	//store
	if (!inst_list[index].execute_started && (inst_list[index].inst_type == STORE)) {
		if (stations[i].Qj == -1 && load_store_q.front().inst_type == STORE) {
			inst_list[index].execute_clock_left = 3;
			inst_list[index].execute_started = true;
			inst_list[index].started_execution_cycle = clock_count;
			//please put it in wb
			// rob[inst_list[index].rob_index] = stations[i].Vj + stations[i].address;
		}
	}
	if (!inst_list[index].execute_started && inst_list[index].inst_type == BEQ) {
		if (stations[i].Qj == -1 && stations[i].Qk == -1) {

			inst_list[index].execute_clock_left = 1;
			inst_list[index].execute_started = true;
			inst_list[index].started_execution_cycle = clock_count;

		}

	}
	if (!inst_list[index].execute_started && (inst_list[index].inst_type == JAL || inst_list[index].inst_type == JMP)) {
		inst_list[index].execute_clock_left = 1;
		inst_list[index].execute_started = true;
		inst_list[index].started_execution_cycle = clock_count;

	}
	if (!inst_list[index].execute_started && inst_list[index].inst_type == RET) {
		if (stations[i].Qj == -1) {

			inst_list[index].execute_clock_left = 1;
			inst_list[index].execute_started = true;
			inst_list[index].started_execution_cycle = clock_count;
		}
	}


}


void wb(instructions* instruction_list, int index) {
	int result =0;
	cout << "I'm in wb stage of " <<index<< endl;
	int RS_index = instruction_list[index].rs_index;
	if (!(common_data_bus_1_busy && common_data_bus_2_busy)) {
		//CDB Reservation
		if (!common_data_bus_1_busy) common_data_bus_1_busy = true;
		else if (!common_data_bus_2_busy) common_data_bus_2_busy = true;
		
		reorder_buffer[instruction_list[index].rob_index].ready = true;
		stations[RS_index].busy = false;
		instruction_list[index].written_cycle = clock_count;
		if (instruction_list[index].inst_type == ADD || instruction_list[index].inst_type == ADDI) {
			result = stations[RS_index].Vj + stations[RS_index].Vk;
		}
		else if(instruction_list[index].inst_type == SUB) {
			result = stations[RS_index].Vj - stations[RS_index].Vk;
		}
		else if (instruction_list[index].inst_type == NAND) {
			result = ~(stations[RS_index].Vj & stations[RS_index].Vk);
		}
		else if (instruction_list[index].inst_type == MUL) {
			result = stations[RS_index].Vj * stations[RS_index].Vk;
		}
		else if (instruction_list[index].inst_type == LOAD) {
			result = MemoryIn_map[stations[RS_index].address];
		}
		else if (instruction_list[index].inst_type == STORE) {
			result = 0;
			reorder_buffer[instruction_list[index].rob_index].Dest = stations[RS_index].Vj + stations[RS_index].address;
			if (stations[RS_index].Qk == -1) {
				result = stations[RS_index].Vk;
			}
		}
		else if (instruction_list[index].inst_type == RET) {
			result = stations[RS_index].Vj;
		}
		else if (instruction_list[index].inst_type == JAL || instruction_list[index].inst_type == JMP) {
			result = stations[RS_index].address;
		}
		else if (instruction_list[index].inst_type == BEQ) {
			result = stations[RS_index].Vj - stations[RS_index].Vk;
		}


		reorder_buffer[instruction_list[index].rob_index].value = result;


		for (int i = 0; i < 15; i++) {
			if (stations[i].busy) {
				if (!(instruction_list[index].inst_type == STORE)) {
					if (stations[i].Qj == stations[RS_index].Dest) {
						stations[i].Vj = result;
							stations[i].Qj = -1;
							cout << "qj ready" << endl;
					}
				}
				if (stations[i].Qk == stations[RS_index].Dest) {
					stations[i].Vk = result;
					stations[i].Qk = -1;
					cout << "qk ready" << endl;
				}
			}
		}

		instruction_list[index].wb_flag = false;
		instruction_list[index].commit_flag = true;
	}
}
int commit(instructions* instruction_list, int index) {
	int destination = reorder_buffer[instruction_list[index].rob_index].Dest;
	int branch_address;

	if ((rob_head == instruction_list[index].rob_index) && reorder_buffer[rob_head].ready == true ){
		if (!(commit_1_busy && commit_2_busy)) {
			reorder_buffer[instruction_list[index].rob_index].empty = true;

			//Commit Reservation
			if (!commit_1_busy) commit_1_busy = true;
			else if (!commit_2_busy) commit_2_busy = true;

			if (Register_Status[destination] == instruction_list[index].rob_index) {
				Register_Status[destination] = 0;
			}

			if (reorder_buffer[rob_head].inst == BEQ) {
				if (!reorder_buffer[instruction_list[index].rob_index].value) {
					flush_rob();
					branch = true;
					if (instruction_list[index].imm > index) {
						branch_miss++;
					}
					else {
						branch_hit++;
					}
					return instruction_list[index].imm;
				}
			}
			else if (reorder_buffer[rob_head].inst == STORE) {
				MemoryIn_map.insert({ reorder_buffer[rob_head].Dest , reorder_buffer[rob_head].value });
			}
			else if(reorder_buffer[rob_head].inst == JMP) {
				flush_rob();
				branch = true;
				cout << "committed " << index << endl;
				instruction_list[index].commit_cycle = clock_count;
				instruction_list[index].commit_flag = false;
				return instruction_list[index].imm;
			}
			else if (reorder_buffer[rob_head].inst == JAL) {
				flush_rob();
				branch = true;
				cout << "committed " << index << endl;
				instruction_list[index].commit_cycle = clock_count;
				instruction_list[index].commit_flag = false;
				Registers[1] = index + 1;
				return instruction_list[index].imm;
			}
			else if (reorder_buffer[rob_head].inst == RET) {
				flush_rob();
				branch = true;
				return Registers[1];
			}
			else {
				Registers[destination] = reorder_buffer[instruction_list[index].rob_index].value;
			}
			cout << "committed " << index << endl;
			instruction_list[index].commit_cycle = clock_count;
			instruction_list[index].commit_flag = false;
			if (rob_head == 8) {
				rob_head = 1;
			}
			else {
				rob_head++;
			}
		}


	}
	return 0;
}
void flush_rob() {
	for (int i = 1; i < 9; i++) {
		reorder_buffer[i].empty = true;
	}
	rob_head = 1;
	rob_tail = 1;
	for (int j = 0; j < 15; j++) {
		stations[j].busy = false;
		stations[j].Qj = -1;
		stations[j].Qk = -1;
	}
}
void write() {
	ofstream outfile;
	outfile.open("RegisterOutput.txt", ios::app);
	if (outfile.is_open()) {
		outfile << "................................." << endl;
		for (int i = 0; i < 8; i++) {
			outfile << Reg_num[i] << "  Decimal: " << dec << Registers[i] << endl;
			//outfile << "    Hex: 0x" << hex << Registers[i] << endl;
			//bitset<32> b(Registers[i]);
			//outfile << "    Binary: " << b << endl;
		}
		for (int i = 0; i < 15; i++) {
			outfile << "station num" << i << endl;
			outfile << "busy" << stations[i].busy << endl;
			outfile << "instruction type" << stations[i].operation << endl;
			outfile << "vj" << stations[i].Vj << endl;
			outfile << "vk" << stations[i].Vk << endl;
			outfile << "qj" << stations[i].Qj << endl;
			outfile << "qk" << stations[i].Qk << endl;
			outfile << "dest" << stations[i].Dest << endl;
			outfile << "address" << stations[i].address << endl;

			outfile << "address" << stations[i].address << endl;
			outfile << "......."<< endl;

			//outfile << "    Hex: 0x" << hex << Registers[i] << endl;
			//bitset<32> b(Registers[i]);
			//outfile << "    Binary: " << b << endl;
		}
		int pcindex = (pc * 4) + MemoryIn_map[0001];
//		outfile << "R1 status " << Register_Status[1] << endl;

		outfile << "pc" << "  Decimal: " << dec << pcindex << endl;
		//outfile << "    Hex: 0x" << hex << pcindex << endl;
		//bitset<32> b(pcindex);
		//outfile << "    Binary: " << b << endl;
		//outfile.close();
	}
	else
		cout << "cann't open the output file";
}


void write_filler() {
	ofstream outfile;

		outfile.open("Output_clock.csv", ios::app);
		if (outfile.is_open()) {
			outfile << "instruction index" <<","<<"instruction" << "," << "issued" << "," << "started executing" << "," << "finished executing" << "," << "write back" << "," << "committed" << endl;
			for (int i = 0; i < pc; i++) {
				outfile << i<< ","<< func_name[instruction_list[i].inst_type] <<"," << instruction_list[i].issued_cycle << ","
					<< instruction_list[i].started_execution_cycle << "," << instruction_list[i].finished_execution_cycle << ","
					<< instruction_list[i].written_cycle << "," 
					<< instruction_list[i].commit_cycle << endl;
			}
			double ii = pc;
			outfile << "total execution time," << instruction_list[pc-1].commit_cycle << "," << "cycles" << endl;
			outfile << "IPC," << ii / instruction_list[pc-1].commit_cycle << endl;
			if(!(branch_miss + branch_hit))	outfile << "branch miss,0" << endl;
			else outfile << "branch miss," << branch_miss / (branch_miss + branch_hit) << endl;
			outfile.close();
			cout << "cloooooooooooosed" << endl;
		}
		else{
			cout << "cann't open the output file";
	}

}
void print_Memory_map()
{    //printing Memory Map for debugging
	cout << "KEY\tELEMENT\n";
	for (auto itr = MemoryIn_map.begin(); itr != MemoryIn_map.end(); ++itr) {
		cout << itr->first << '\t' << itr->second << '\n';
	}
}
private:
bool check_empty_station(Station_Name station , int &empty_index){
	if (station == LOAD_st) {
		if (!stations[0].busy) {
			empty_index = 0;
			return true;
		}
		else if (!stations[1].busy) {
			empty_index = 1;
			return true;
		}
		else {
			empty_index = -1;
			return false;
		}
	}
	else if(station == STPRE_st) {
		if (!stations[2].busy) {
			empty_index = 2;
			return true;
		}
		else if (!stations[3].busy) {
			empty_index = 3;
			return true;
		}
		else {
			empty_index = -1;
			return false;
		}
	}
	else if (station == UJ_st) {
		if (!stations[4].busy) {
			empty_index = 4;
			return true;
		}
		else if (!stations[5].busy) {
			empty_index = 5;
			return true;
		}
		else {
			empty_index = -1;
			return false;
		}
	}
	else if (station == BEQ_st) {
		if (!stations[6].busy) {
			empty_index = 6;
			return true;
		}
		else if (!stations[7].busy) {
			empty_index = 7;
			return true;
		}
		else {
			empty_index = -1;
			return false;
		}
	}
	else if (station == R_st) {
		if (!stations[8].busy) {
			empty_index = 8;
			return true;
		}
		else if (!stations[9].busy) {
			empty_index = 9;
			return true;
		}
		else if (!stations[10].busy) {
			empty_index = 10;
			return true;
		}
		else {
			empty_index = -1;
			return false;
		}
	}
	else if (station == NAND_st) {
		if (!stations[11].busy) {
			empty_index = 11;
			return true;
		}
		else if (!stations[12].busy) {
			empty_index = 12;
			return true;
		}
		else {
			empty_index = -1;
			return false;
		}
	}
	else if (station == MUL_st) {
		if (!stations[13].busy) {
			empty_index = 13;
			return true;
		}
		else if (!stations[14].busy) {
			empty_index = 14;
			return true;
		}
		else {
			empty_index = -1;
			return false;
		}
	}


	}



bool is_rob_full() {
	if (rob_head == rob_tail && reorder_buffer[rob_head].empty == false) {
		return true;
	}
	return false;
}


bool ROB_store_address(int rob_index, int load_address) {
	int i = rob_head;
	while (i != rob_index) {
		if (reorder_buffer[i].Dest == load_address && (reorder_buffer[i].inst == STORE)) {
			return true;
		}
		if (i == 8) {
			i = 1;
		}
		else {
			i++;
		}

	}
	return false;
}

};



int main()
{
	Tom_assembler as;

	as.read_file("test2.txt");
	as.read_memory("mem.txt");
	as.parse(0);
	as.process();
	as.write();
	as.print_Memory_map();
	as.write_filler();
    //while (!program_done) { 
    //    issue();
    //    update_execution();
    //    clock_sig++;
    //    save&print();
    //    checkifdone();
    //}	
	return 0;
   // print();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
