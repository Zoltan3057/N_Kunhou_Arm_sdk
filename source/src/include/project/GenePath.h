
#ifndef _GENE_PATH_H_
#define _GENE_PATH_H_

#include "math.h"
#include <vector>
#include <RobotStruct.h>

class GenePath
{
public:
	GenePath();
	~GenePath();

public:
	bool GenePathPnts(const std::vector<SOdomSpeed> &input_points,
		              std::vector<SOdomSpeed> &out_points);

private:
	void getPntInLine(SOdomSpeed               start_pnt,
		              SOdomSpeed               end_pnt,
		              std::vector<SOdomSpeed> &pnts);

};




#endif // _GENE_PATH_H_
