#include<fstream>
#include<iostream>
using namespace std;
int main()
{
	ifstream fin;
	string Ipath;
	Ipath = "D:/src.txt";
	//cin >> Ipath;
	Ipath.assign(Ipath);
	fin.open(Ipath);

	ofstream fout;
	string Opath;
	Opath = "D:/dst.asm";
	//cin >> Opath;
	Opath.assign(Opath);
	fout.open(Opath);

	unsigned int inst;
	int cnt = -1;
	while (!fin.eof()) {
		++cnt;
		fout << "TAG" << cnt << ":";
		fin >> hex >> inst;
		unsigned i50 = inst << 26;
		i50 = i50 >> 26;
		unsigned rd = inst << 16;
		rd = rd >> 27;
		unsigned rt = inst << 11;
		rt = rt >> 27;
		unsigned sa = inst << 21;
		sa = sa >> 27;
		unsigned im = inst << 16;
		im = im >> 16;
		unsigned add = inst << 6;
		add = add >> 6;
		unsigned rs = inst << 6;
		rs = rs >> 27;
		signed ims= inst << 16;
		ims = ims >> 16;
		unsigned in3126 = inst >> 26;
		if (inst == 0x42000018) {
			fout << "ERET\n";
			goto next_inst;
		}
		switch (in3126) {
			case 0:
				if (i50 == 0b10000 && rt == 0 && sa == 0 && rs == 0) {
					//mfhi
					fout << "MFHI $" << rd << endl;
					goto next_inst;
				}
				if (i50 == 0b10010 && rt == 0 && sa == 0 && rs == 0) {
					//mflo
					fout << "MFLO $" << rd << endl;
					goto next_inst;
				}
				if (rt == 0 && rd == 0 && sa == 0 && i50 == 0b10001) {
					fout << "MTHI $" << rs << endl;
					goto next_inst;
				}
				if (rt == 0 && rd == 0 && sa == 0 && i50 == 0b10011) {
					fout << "MTLO $" << rs << endl;
					goto next_inst;
				}
				if (inst >> 21 == 0) {
					if (((inst << 26) >> 26) == 0) {
						fout << "SLL ";
						goto rrs;
					}
					else if (((inst << 26) >> 26) == 0b10) {
						fout << "SRL ";
						goto rrs;
					}
					else if (((inst << 26) >> 26) == 0b11) {
						fout << "SRA ";
						rrs:
						fout << "$" << rd << ",$" << rt << "," << sa << endl;
						goto next_inst;
					}
				}//sll,srl,sra
				if ((inst << 11) == (0b1000 << 11)) {
					fout << "JR $";
					fout << (inst >> 21)<<endl;
					goto next_inst;
				}
				if (rt == 0 && i50 == 0b1001) {
					fout << "JALR ";
					fout << "$" << rd << ",$" << rs << endl;
					goto next_inst;
				}
				if (rd == 0 && (inst << 21) == (0b11010 << 21)) {
					//div
					fout << "DIV ";
					goto rsrt;
				}
				if (rd == 0 && (inst << 26) == (0b11011 << 26)) {
					//divu
					fout << "DIVU ";
					goto rsrt;
				}
				if (rd == 0 && (inst << 26) == (0b11000 << 26)) {
					//mul
					fout << "MULT ";
					goto rsrt;
				}
				if (rd == 0 && (inst << 26) == (0b11001 << 26)) {
					//mulu
					fout << "MULTU ";
				rsrt:
					fout << "$" << rs << ",$" << rt << endl;
					goto next_inst;
				}
				switch ((inst << 21)>>21) {
					case 0b1101:
						fout << "BREAK\n";
						goto next_inst;
					case 0b1100:
						fout << "SYSCALL\n";
						goto next_inst;
					case 0b110100:
						fout << "TEQ $" << rs << ",$" << rt << endl;
						goto next_inst;
					case 0b100000:
						fout << "ADD ";
						goto rrr;
					case 0b100001:
						fout << "ADDU ";
						goto rrr;
					case 0b100010:
						fout << "SUB ";
						goto rrr;
					case 0b100011:
						fout << "SUBU ";
						goto rrr;
					case 0b100100:
						fout << "AND ";
						goto rrr;
					case 0b100101:
						fout << "OR ";
						goto rrr;
					case 0b100110:
						fout << "XOR ";
						goto rrr;
					case 0b100111:
						fout << "NOR ";
						goto rrr;
					case 0b101010:
						fout << "SLT ";
						goto rrr;
					case 0b101011:
						fout << "SLTU ";
					rrr:
						fout << "$" << rd << ",$" << rs << ",$" << rt << endl;
						goto next_inst;
					case 0b000100:
						fout << "SLLV ";
						goto rrrdts;
					case 0b110:
						fout << "SRLV ";
						goto rrrdts;
					case 0b111:
						fout << "SRAV ";
					rrrdts:
						fout << "$" << rd << ",$" << rt << ",$" << rs << endl;
						goto next_inst;
					default:
						goto next_inst;
				}//switch ins10:0
				break;
			case 0b1:
				if (rt==0b1) {
					fout << "BGEZ ";
					fout << "$" << rs << ","
						<< "TAG" << cnt + 1 + ims << endl;
					goto next_inst;
				}
				goto next_inst;
			case 0b11100:
				if (sa == 0 && i50 == 0b100000)
					fout << "CLZ $" << rd << ",$" << rs << endl;
				else if (sa == 0 && i50 == 0b10) {
					fout << "MUL ";
					goto rrr;
				}
				goto next_inst;
			case 0b1000:
				fout << "ADDI ";
				goto rris;
			case 0b1001:
				fout << "ADDIU ";
				goto rris;
			case 0b1100:
				fout << "ANDI ";
				goto rri;
			case 0b1101:
				fout << "ORI ";
				goto rri;
			case 0b1110:
				fout << "XORI ";
				goto rri;
			case 0b10000:
				if (sa == 0 && i50 == 0) {
					if (rs == 0)
						fout << "MFC0 $" << rt << ",$" << rd << endl;
					else if (rs == 0b100)
						fout << "MTC0 $" << rt << ",$" << rd << endl;
				}
				goto next_inst;
			case 0b100011:
				fout << "LW ";
				goto rri2;
			case 0b101011:
				fout << "SW ";
			rri2:
				fout << "$" << rt << "," << ims << "($" << rs << ")\n";
				goto next_inst;
			case 0b100:
				fout << "BEQ ";
				goto rris2;
			case 0b101:
				fout << "BNE ";
			rris2:
				fout << "$" << rs << ",$" << rt << ","
					<< "TAG" << cnt + 1 + ims << endl;
				goto next_inst;
			case 0b1010:
				fout << "SLTI ";
			rris:
				fout << "$" << rt << ",$" << rs << "," << ims << endl;
				goto next_inst;
			case 0b1011:
				fout << "SLTIU ";
			rri:
				fout << "$" << rt << ",$" << rs << "," << im<<endl;
				goto next_inst;
			case 0b1111:
				if (inst >> 21 == 0b111100000) {
					fout << "LUI ";
					unsigned rt = inst << 11;
					rt = rt >> 27;
					unsigned im = inst << 16;
					im = im >> 16;
					fout << "$" << rt << "," << im << endl;
				}
				goto next_inst;
			case 0b10:
				fout << "J ";
				fout << "TAG" << (4 * add - 0x400000) / 4 << endl;
				goto next_inst;
			case 0b11:
				fout << "JAL ";
				fout << "TAG" << (4 * add - 0x400000) / 4 << endl;
				goto next_inst;
			case 0b100100:
				fout << "LBU ";
				goto rri2;
			case 0b100101:
				fout << "LHU ";
				goto rri2;
			case 0b100000:
				fout << "LB ";
				goto rri2;
			case 0b100001:
				fout << "LH ";
				goto rri2;
			case 0b101000:
				fout << "SB ";
				goto rri2;
			case 0b101001:
				fout << "SH ";
				goto rri2;
			default:
				fout << "#%///*Not included*/\n";
				goto next_inst;
		}//switch ins31:26
	next_inst:
		;
	}

	fin.close();
	fout.close();
	return 0;
}
