/*
 * moons_TPDO.h
 *
 *  Created on: 17/06/2017
 *      Author: edwardzhang
 */

#ifndef MOONS_TPDO_H_
#define MOONS_TPDO_H_

#include <boost/thread.hpp>

#include "TimerDiff.h"
#include "chassis/driver_base.h"

typedef struct  _moons_Can_data
{
	//feedback
	S32 left_pos;
	S32 right_pos;


}moons_Can_data;


class pdo_data
{
public:
	pdo_data();
	~pdo_data();

	void state_machine(U8* data);
	moons_Can_data get_data();


protected:


private://virtual customized :TODO
	void clear_machine();

	typedef enum{HEAD,SE,RL,LEN,ID,DATA} Package;
	Package pack;

	U32 COB_ID_;
	U8  Data8[8];
	U8  id_len_;
	U8  data_len_;

	S32 leftPos;
	S32 rightPos;

	boost::mutex mu_data_;
	void parse_data();
	void parse_281();
	void parse_282();


private:
	moons_Can_data can_data_;
};



#endif /* MOONS_TPDO_H_ */
