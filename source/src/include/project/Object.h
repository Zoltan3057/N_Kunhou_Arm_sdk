#ifndef AGV_SIMULATER_OBJECT_20121101_
#define AGV_SIMULATER_OBJECT_20121101_

#include <map>
#include "MyDefine.h"

class Object
{
public:
	Object();
	~Object();

	virtual int type() const = 0;

	//typedef enum{NONE,DATA,FRAME,TRANSFER,NODE_TOPOLOGY,EDGE_LINE,EDGE_BEZIER} Otype;
	typedef enum{NONE,DATA,FRAME,TRANSFER,NODE,INIT_NODE,EDGE,TASK_NODE,TASK_EDGE,TASK_LIST} Otype;
	
	U32 ID();

	virtual void Code(std::ofstream& of)=0;
	virtual bool DeCode(std::string data)=0;

	void Init();
	void Init(U32 id);

	static void ClearType(int type);		//�������
private:
	
	static U32 GetNewID(int type);	//������id
	
	U32 id_;

	static std::map<int,U32> id_arrange_list_;//id�ŷ��� 

};

#endif //AGV_SIMULATER_OBJECT_20121101_
