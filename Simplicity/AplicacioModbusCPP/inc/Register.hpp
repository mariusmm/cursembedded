/*
 * Register.hpp
 *
 *  Created on: 21 de des. 2020
 *      Author: marius
 */

#ifndef INC_REGISTER_HPP_
#define INC_REGISTER_HPP_

#include <definitions.hpp>
#include <cstdint>

class Register_cb {
public:
	virtual void callback(unsigned int reg_num, int data) = 0;
	virtual ~Register_cb() {};
};

class Register{
public:
  Register();
  bool store(void);
  uint16_t direct_read(unsigned int reg_num);
  void direct_write (unsigned int reg_num, uint16_t data);
  uint16_t read(unsigned int reg_num);
  bool write(unsigned int reg_num, uint16_t data);

  bool register_wr_callback( unsigned int reg_num, void func(int, int));
  bool register_wr_callback( unsigned int reg_num, Register_cb *cb);

  bool register_rd_callback( unsigned int reg_num, void func(int));

private:
  uint16_t m_data[NUM_REGISTER_FILE + 1];
  void (*m_getter[NUM_REGISTER_FILE])(int) = {nullptr} ;
  Register_cb *m_setter[NUM_REGISTER_FILE] = {nullptr};
  bool m_permanent[NUM_REGISTER_FILE] = {false};
};



#endif /* INC_REGISTER_HPP_ */
