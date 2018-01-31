/*
*  Polar Scan Matcher
*  Copyright (C) 2010, CCNY Robotics Lab
*  Ivan Dryanovski <ivan.dryanovski@gmail.com>
*  William Morris <morris@ee.ccny.cuny.edu>
*  http://robotics.ccny.cuny.edu
*  Modified 2014, Daniel Axtens <daniel@axtens.net>
*  whilst a student at the Australian National University
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  This is a wrapper around Polar Scan Matcher [1], written by 
*  Albert Diosi
*
*  [1] A. Diosi and L. Kleeman, "Laser Scan Matching in Polar Coordinates with 
*  Application to SLAM " Proceedings of 2005 IEEE/RSJ International Conference 
*  on Intelligent Robots and Systems, August, 2005, Edmonton, Canada
*/

#ifndef POLAR_SCAN_MATCHER_PSM_NODE_H
#define POLAR_SCAN_MATCHER_PSM_NODE_H

#include "RobotStruct.h"

#include "scanmatcher/polar_match.h"


const double ROS_TO_PM = 100.0;   // convert from cm to m

class PSMNode
{
  private:

    bool initialized_;

    PolarMatcher matcher_;
    PMScan * prevPMScan_;

	boost::mutex odomMutex_;
    boost::mutex imuMutex_;
    double prevImuAngle_;   // the yaw angle when we last perfomred a scan match
    double currImuAngle_;   // the most recent yaw angle we have received

    // **** parameters

    bool   useTfOdometry_;
    bool   useImuOdometry_;

    int    minValidPoints_;
    int    searchWindow_;
    double maxError_;
    int    maxIterations_;
    double stopCondition_;


    void getParams();
    bool initialize(const SLaser &scan);


	//SOdomSpeed last_odom_;

	//SPos EstimatedPos_;
	bool b_do_scan_matcher_;
	//bool b_first_odom_;
	F32 laser_rotation_;


    void rosToPMScan(const SLaser &scan, 
                     const SPos &pos,
                        PMScan* pmScan);
 

  public:

    PSMNode();
    virtual ~PSMNode();

// 	void odomCallback (const SOdomSpeed &odomMsg);
// 	void imuCallback (const int &imuMsg);
// 	bool scanCallback (SPos &new_diff,const SLaser& scan);
	bool scanMatcher(SPos &est_pos,const SLaser &cur_scan);
	bool optimize(SPos &est_pos,const SLaser &ref_scan,const SLaser &cur_scan);
	bool optimize( SPos &est_pos,const SLaser &cur_scan );

	void setShowFunction(boost::function< void(const SLaserXY &laser)> fnc){
		matcher_.setShowFunction(fnc);
	};


};

#endif // POLAR_SCAN_MATCHER_PSM_NODE_H
