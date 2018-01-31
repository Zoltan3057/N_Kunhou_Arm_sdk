#ifndef _CUI_KYOSHO_20151008_
#define _CUI_KYOSHO_20151008_

#include "MyDefine.h"

class CUI
{
public:
	CUI();
	~CUI();

	void init(F32 v_acc, F32 v_dec, F32 w_acc, F32 w_dec);

	bool set_keycode( char keycode ,F32 &cvx , F32 &cw);
	

protected:

private:
	F32 v_acc_;
	F32 v_dec_;
	F32 w_acc_;
	F32 w_dec_;

	F32 max_v_;
	F32 max_w_;

	F32 fast_k_;

	F32 cvx_;
	F32 cw_;
};


#endif//_CUI_KYOSHO_20151008_