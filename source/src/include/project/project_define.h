#ifndef _PROJECT_DEFINE_KYOSHO_20160910_
#define _PROJECT_DEFINE_KYOSHO_20160910_

#include <map>
#include <string>
#include "MyDefine.h"
#include "RobotStruct.h"
#include "Object.h"


class Node_base;
class Edeg_base;


typedef enum{EDGE_FRONT,EDGE_BACK,EDGE_BOTH} emEdgeDirType;
// typedef enum{NODE_BASE} emNodeType;
// typedef enum{EDGE_BASE,EDGE_LINE,EDGE_BEZIER} emEdgeType;

typedef struct _EDITION
{
	U32 main;
	U32 sub;

}EDITION;


#endif//_PROJECT_DEFINE_KYOSHO_20160910_
