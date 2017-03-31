#include <stdio.h>
#include <string.h>
#include "common.hpp"
#include "instruction/base.hpp"
#include "emulator/emulator.hpp"

#define MEMORY_SIZE (1*MB)

int main(int argc, char *argv[]){
	Emulator emu(MEMORY_SIZE, 0xf000, 0xfff0);
	Instr16 instr16(&emu);
	Instr32 instr32(&emu);

	emu.load_binary("bios/crt0.bin", 0xffff0, 0x10);
	emu.load_binary("bios/bios.bin", 0xf0000, 0x400);
	emu.load_binary(argv[1], 0x7c00, 0x200);

	//while(!emu.is_halt() && emu.get_eip()){
	while(!emu.is_halt()){
		bool is_protected = emu.is_protected();

		try{
			uint8_t chsz;
			bool chsz_op, chsz_ad;

			emu.hundle_interrupt();

			chsz = (is_protected ? instr32.chk_chsz() : instr16.chk_chsz());
			chsz_op = chsz & CHSZ_OP;
			chsz_ad = chsz & CHSZ_AD;

			if(is_protected ^ chsz_op){
				instr32.set_chsz_ad(!(is_protected ^ chsz_ad));
				instr32.parse();
				instr32.exec();
			}
			else{
				instr16.set_chsz_ad(is_protected ^ chsz_ad);
				instr16.parse();
				instr16.exec();
			}
		}
		catch(int n){
			emu.dump_regs();
			ERROR("Exception %d", n);
			emu.queue_interrupt(n, true);
		}
		/*
		emu.dump_regs();
		if((emu.get_sgreg(SS)<<4)+emu.get_gpreg(ESP)>0x40)
			emu.dump_mem((emu.get_sgreg(SS)<<4)+emu.get_gpreg(ESP)-0x40, 0x80);
		MSG("\n");
		*/

	}
	emu.dump_regs();
	emu.dump_mem((emu.get_sgreg(SS)<<4)+emu.get_gpreg(ESP)-0x40, 0x80);
}
