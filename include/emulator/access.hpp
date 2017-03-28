#ifndef _DATA_ACCESS_H
#define _DATA_ACCESS_H

#include <stdint.h>
#include "common.hpp"
#include "hardware/hardware.hpp"

enum acsmode_t { MODE_READ, MODE_WRITE, MODE_EXEC };

class DataAccess : public virtual Hardware {
	public:
		uint8_t get_data8(sgreg_t seg, uint32_t addr){ return read_mem8_seg(seg, addr); };
		uint16_t get_data16(sgreg_t seg, uint32_t addr){ return read_mem16_seg(seg, addr); };
		uint32_t get_data32(sgreg_t seg, uint32_t addr){ return read_mem32_seg(seg, addr); };

		void put_data8(sgreg_t seg, uint32_t addr, uint8_t v){ write_mem8_seg(seg, addr, v); };
		void put_data16(sgreg_t seg, uint32_t addr, uint16_t v){ write_mem16_seg(seg, addr, v); };
		void put_data32(sgreg_t seg, uint32_t addr, uint32_t v){ write_mem32_seg(seg, addr, v); };

		uint8_t get_code8(int index){ return exec_mem8_seg(CS, get_eip() + index); };
		uint16_t get_code16(int index){ return exec_mem16_seg(CS, get_eip() + index); };
		uint32_t get_code32(int index){ return exec_mem32_seg(CS, get_eip() + index); };

		void push32(uint32_t v);
		uint32_t pop32(void);
		void push16(uint16_t v);
		uint16_t pop16(void);

	private:
		uint32_t trans_v2p(acsmode_t mode, sgreg_t seg, uint32_t vaddr);
		uint32_t trans_v2l(acsmode_t mode, sgreg_t seg, uint32_t vaddr);

		uint32_t read_mem32_seg(sgreg_t seg, uint32_t addr) { return read_mem32(trans_v2p(MODE_READ, seg, addr)); };
		uint16_t read_mem16_seg(sgreg_t seg, uint32_t addr) { return read_mem16(trans_v2p(MODE_READ, seg, addr)); };
		uint8_t read_mem8_seg(sgreg_t seg, uint32_t addr) { return read_mem8(trans_v2p(MODE_READ, seg, addr)); };
		void write_mem32_seg(sgreg_t seg, uint32_t addr, uint32_t v) { write_mem32(trans_v2p(MODE_WRITE, seg, addr), v); };
		void write_mem16_seg(sgreg_t seg, uint32_t addr, uint16_t v) { write_mem16(trans_v2p(MODE_WRITE, seg, addr), v); };
		void write_mem8_seg(sgreg_t seg, uint32_t addr, uint8_t v) { write_mem8(trans_v2p(MODE_WRITE, seg, addr), v); };
		uint32_t exec_mem32_seg(sgreg_t seg, uint32_t addr) { return read_mem32(trans_v2p(MODE_EXEC, seg, addr)); };
		uint16_t exec_mem16_seg(sgreg_t seg, uint32_t addr) { return read_mem16(trans_v2p(MODE_EXEC, seg, addr)); };
		uint8_t exec_mem8_seg(sgreg_t seg, uint32_t addr) { return read_mem8(trans_v2p(MODE_EXEC, seg, addr)); };
};

#endif