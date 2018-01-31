#ifndef __K_RECOVERY_INTERFACE_HPP__
#define __K_RECOVERY_INTERFACE_HPP__

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <Pluma/Pluma.hpp>

class KRecoveryInterface
{
public:

	virtual void runRecovery() = 0;
	// to be done . how to recover?
	
protected:
	boost::function< void (F32,F32,F32)> fnc_setspeed_;
};

PLUMA_PROVIDER_HEADER_BEGIN(KRecoveryInterface)
	public:
		virtual KRecoveryInterface* create(boost::function< void (F32,F32,F32)> fnc) const = 0;
		virtual std::string getName() = 0;

PLUMA_PROVIDER_HEADER_END

#define CUSTOMED_INHERIT_PROVIDER_RECOVERY(SPECIALIZED_TYPE)				\
class SPECIALIZED_TYPE##Provider: public KRecoveryInterfaceProvider{		\
public:																		\
	KRecoveryInterface * create(boost::function< void (F32,F32,F32)> fnc) const{ return new SPECIALIZED_TYPE (fnc); }	\
	std::string getName(){return std::string(#SPECIALIZED_TYPE);}			\
};

#endif
