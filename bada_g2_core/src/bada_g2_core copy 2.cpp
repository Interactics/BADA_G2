#include "bada_g2_core.h"


void bada_set_state(STATE &present_state, STATE target_state)
{
	int temp_int = int(target_state);
	present_state=static_cast<STATE>(temp_int);
}

void bada_next_state(STATE &present_state)
{
	int temp_int = int(present_state);
	STATE temp_state;
	if (temp_int != int(MOVING_WITH_PEPL))
	{
		temp_int++;
		temp_state = static_cast<STATE>(temp_int);
		present_state = temp_state;
	}
	else if (temp_int != int(MOVING_WITH_PEPL))
	{
		present_state = ROAMING;
	}
}

// send simple goal
void bada_send_destination(double x, double y, double orien_z, double orien_w)
{ //맵 위치 x,y quaternion z,w 를 설정해주고 그 위치로 이동
	//- ACTION MSG 퍼블리시한다.
	// ROS_INFO("waiting for move base");
	actionClient->waitForServer();
	// ROS_INFO("connected to move base");
	move_base_msgs::MoveBaseGoal goal;

	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = x;
	goal.target_pose.pose.position.y = y;
	goal.target_pose.pose.orientation.z = orien_z;
	goal.target_pose.pose.orientation.w = orien_w;

	actionClient->sendGoal(goal);
	return;
}

// send simple goal
void bada_send_destination_demo(double x, double y, double orien_z, double orien_w)
{ //맵 위치 x,y quaternion z,w 를 설정해주고 그 위치로 이동
	//- ACTION MSG 퍼블리시한다.
	// ROS_INFO("waiting for move base");
	actionClient->waitForServer();
	// ROS_INFO("connected to move base");
	move_base_msgs::MoveBaseGoal goal;

	goal.target_pose.header.frame_id = "odom";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = x;
	goal.target_pose.pose.position.y = y;
	goal.target_pose.pose.orientation.z = orien_z;
	goal.target_pose.pose.orientation.w = orien_w;

	actionClient->sendGoal(goal);
	return;
}

void bada_go_destination_blocking(double duration, double x, double y, double orien_z, double orien_w)
{ //맵 위치 x,y quaternion z,w 를 설정해주고 그 위치로 이동
	//- ACTION MSG 퍼블리시한다.
	// ROS_INFO("waiting for move base");
	actionClient->waitForServer();
	// ROS_INFO("connected to move base");
	move_base_msgs::MoveBaseGoal goal;

	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = x;
	goal.target_pose.pose.position.y = y;
	goal.target_pose.pose.orientation.z = orien_z;
	goal.target_pose.pose.orientation.w = orien_w;

	actionClient->sendGoal(goal);

	//http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionClient
	bool finished_before_timeout = actionClient->waitForResult(ros::Duration(duration));

	if (finished_before_timeout)
	{
		actionlib::SimpleClientGoalState state = actionClient->getState();
		ROS_INFO("Action finished: %s", state.toString().c_str());
	}
	else{
		ROS_INFO("Action did not finish before the time out.");
		//-  배회 중단. 액션 메시지 취소 보내기
		actionClient->cancelGoal();
	}
	return;
}


void bada_go_destination_blocking_demo(double duration, double x, double y, double orien_z, double orien_w)
{ //맵 위치 x,y quaternion z,w 를 설정해주고 그 위치로 이동
	//- ACTION MSG 퍼블리시한다.
	// ROS_INFO("waiting for move base");
	actionClient->waitForServer();
	// ROS_INFO("connected to move base");
	move_base_msgs::MoveBaseGoal goal;

	goal.target_pose.header.frame_id = "odom";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = x;
	goal.target_pose.pose.position.y = y;
	goal.target_pose.pose.orientation.z = orien_z;
	goal.target_pose.pose.orientation.w = orien_w;

	actionClient->sendGoal(goal);

	//http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionClient
	bool finished_before_timeout = actionClient->waitForResult(ros::Duration(duration));

	if (finished_before_timeout)
	{
		actionlib::SimpleClientGoalState state = actionClient->getState();
		ROS_INFO("Action finished: %s", state.toString().c_str());
	}
	else{
		ROS_INFO("Action did not finish before the time out.");
		//-  배회 중단. 액션 메시지 취소 보내기
		actionClient->cancelGoal();
	}
	return;
}

// void bada_save_current_position(){ //호출되면 로봇 현재 위치 저장
// 	// TODO: use realsense topic to get angle and theta
// 	tf::StampedTransforms tranform;
// 	geometry_msgs::Pose2D pose2d;

// 	distance, theta=REALSENSEANGLETHETATODO();
// 	// theta: radian
// s	tranform=getCurrentRobotPositionTODO();
// 	double robotX=transform.pose.x;
// 	double robotY=transform.pose.y;

// 	tf::Matrix3x3 m(q);
// 	// https://gist.github.com/marcoarruda/f931232fe3490b7fa20dbb38da1195ac
// 	double roll, pitch, yaw;
// 	m.getRPY(roll, pitch, yaw);

//     pose2d.theta = yaw+theta;
// 	double deltaX=distance*cos(pose2d.theta);
// 	double deltaY=distance*sin(pose2d.theta);

//     pose2d.x = robotX+deltaX;
//     pose2d.y = robotY+deltaY;

// 	return pose2d;
// }

// void getCurrentRobotPositionTODO(){
// 	ros::NodeHandle n;
// 	tf::TransformListener listener;
// 	while(n.ok())
// 	{
// 		tf::StampedTransform transform;
// 		try{
// 			listener.lookupTransform("/map", "/base_link",  ros::Time(0), transform);
// 			CURRENT_ROBOT_POSITION.pose.pose.position.x = transform.getOrigin().x();
// 			CURRENT_ROBOT_POSITION.pose.pose.position.y = transform.getOrigin().y();
// 			CURRENT_ROBOT_POSITION.pose.pose.orientation.z = transform.getRotation().getZ();
// 			CURRENT_ROBOT_POSITION.pose.pose.orientation.w =  transform.getRotation().getW();
// 		}
// 		catch (tf::TransformException ex){
// 			ROS_ERROR("%s",ex.what());
// 			ros::Duration(1.0).sleep();
//     	}
// 	}
// };

bool bada_rounding()
{
	geometry_msgs::Twist msg; // 회전하기 위해 퍼블리시 용도로 만들어진 변수
	//subscribing_odometry
	//Save present angle
	// geometry_msgs::Quaternion initial_angle = CURRENT_ROBOT_POSITION.pose.pose.orientation;        // 현재 각도 정보를 저장
	//http://docs.ros.org/melodic/api/nav_msgs/html/msg/Odometry.html

	msg.angular.z = (3.14f / 10.0f); // 회전하도록하기

	bada_open_eyes_cmd(true); // 눈 뜨기. (정보 받기 시작)
	ros::Rate rate(5);		  // ROS Rate at 5Hz 0.2 sec

	int time = 0;
	float Ang_Position = 0;

	do
	{
		// ROS_INFO("spinning");

		pub_cmdvel.publish(msg); //각속도 정보 pub, 통신 실패를 방지하기 위해 while문에 넣어놓음.
		ros::spinOnce();		 // bada/eyes로부터 토픽 서브스크라이빙, 현재 오도메트리 정보 서브스크라이빙 목적으로 스핀
		// msg.angular.z;
		if (PPL_CHECK)
		{ // 만약 사람 정보가 ROI에 들어왔다면 true
			ROS_INFO("person detected.");
			//BOOKMARK1
			// TODO : USE ROBOT POSITION
			ROS_INFO("get ROBOT pose");
			SAVED_HUMAN_POSITION = bada_get_robot_pos();
			ROS_INFO("get ROBOT pose done");
			/** TODO : 각도와 거리를 이용하여 포인트를 저장한다.  **/
			// ~~맵에 사람의 위치 포인트를 저장하는 방법, 즉 데이터타입이 무엇인지 알아볼 것. ~~<<-- 사람 위치 저장하지 말 것
			// 지금 위치를 저장한다. (로봇의 위치) <<-- 이것을 사용할것

			// init values
			PPL_ANGLE = -90;	   // Angle of PPL respect to camera
			PPL_DIST = -1.0;	   // Distnace to PPL from

			bada_display_cmd(DISP_EVNT::EXCLAMATION);
			waitSec(0.3);
			pub_cmdvel.publish(msg); //스탑
			bada_display_cmd(DISP_EVNT::NOTHING);
			waitSec(0.3);
			pub_cmdvel.publish(msg); //스탑
			bada_display_cmd(DISP_EVNT::EXCLAMATION);
			waitSec(0.3);
			bada_display_cmd(DISP_EVNT::NOTHING);

			break;
		}
		rate.sleep(); // 6헤르츠가 적당할 듯. 연산 과부화 방지용.
		time++;
		Ang_Position = (time * 0.2) * msg.angular.z;
		// ROS_INFO("%f",Ang_Position);
	} while (ros::ok() && Ang_Position < 6.28); //한바퀴 돌았는지? 6.28
	bada_open_eyes_cmd(false);

	msg.angular.z = 0.0;
	pub_cmdvel.publish(msg); //스탑
	waitSec(0.2);
	pub_cmdvel.publish(msg); //스탑
	waitSec(0.2);

	if (PPL_CHECK){
		ROS_INFO("Person CHECKED");
		PPL_CHECK = false;	   // Is there PPL?
		return true;
	}
	else{
		ROS_INFO("Person no");
		PPL_CHECK = false;	   // Is there PPL?
		return false;
	}
}

void sub_sig_checker_callback(const std_msgs::Empty &msg)
{
	SIG_CHECK = true;
} // 소리가 발생하는지 체크하는 콜백함수

void bada_roaming()
{	//현재위치에서 마지막 지점까지 이동
	// bada_go_destination_blocking();			        //다음 지점으로 이동하기.

	ros::Rate loop_rate(1);
	int t=0;
	CURRENT_POINT += 1;
	for (; CURRENT_POINT <= 3; CURRENT_POINT++)
	{
		CURRENT_POINT %= 3;

		//while(ros::Duration(30))
		bada_send_destination(wayPoint[CURRENT_POINT][0], wayPoint[CURRENT_POINT][1], wayPoint[CURRENT_POINT][2], wayPoint[CURRENT_POINT][3]);
		// TODO : Investigate why while lo op breaks..
		ROS_INFO("SENDING DESTINATION");
		t=0;
		while (ros::ok() && t<100)
		{
			t++;
			loop_rate.sleep();
			ros::spinOnce(); //소리 검사 결과 받기.
			//ROS_INFO("roaming");
			if (SIG_CHECK)
			{
				SIG_CHECK = false;
				ROS_INFO("sig check");

				// bada_save_sound_odom();	                    //- robot pos 정보 오도메트리 저장하기.
				//-  배회 중단. 액션 메시지 취소 보내기
				actionClient->cancelGoal();
				return;
			}
			if (actionClient->getState() == actionlib::SimpleClientGoalState::SUCCEEDED /*--목표도착-- ACTION*/)
			{
				ROS_INFO("%d goal reach", CURRENT_POINT);
				// bada_go_destination_blocking();			//다음 지점으로 이동하기.
				break;
			}
		}
		if(!ros::ok()) break;
	}
	ROS_INFO("roaming done");
}


void bada_roaming_demo()
{	//현재위치에서 마지막 지점까지 이동
	// bada_go_destination_blocking();			        //다음 지점으로 이동하기.

	ros::Rate loop_rate(1);
	int t=0;
	CURRENT_POINT += 1;
	for (; CURRENT_POINT <= 2; CURRENT_POINT++)
	{
		CURRENT_POINT %= 2;

		//while(ros::Duration(30))
		bada_send_destination_demo(wayPoint[CURRENT_POINT][0], wayPoint[CURRENT_POINT][1], wayPoint[CURRENT_POINT][2], wayPoint[CURRENT_POINT][3]);
		// TODO : Investigate why while lo op breaks..
		ROS_INFO("SENDING DESTINATION");
		t=0;
		while (ros::ok() && t<100)
		{
			t++;
			loop_rate.sleep();
			ros::spinOnce(); //소리 검사 결과 받기.
			//ROS_INFO("roaming");
			if (SIG_CHECK)
			{
				SIG_CHECK = false;
				ROS_INFO("sig check");

				// bada_save_sound_odom();	                    //- robot pos 정보 오도메트리 저장하기.
				//-  배회 중단. 액션 메시지 취소 보내기
				actionClient->cancelGoal();
				return;
			}
			if (actionClient->getState() == actionlib::SimpleClientGoalState::SUCCEEDED /*--목표도착-- ACTION*/)
			{
				ROS_INFO("%d goal reach", CURRENT_POINT);
				// bada_go_destination_blocking();			//다음 지점으로 이동하기.
				break;
			}
		}
		if(!ros::ok()) break;
	}
	ROS_INFO("roaming done");
}

void bada_go_to_pepl()
{
	// while
	bada_go_destination_blocking(
		100.0,
		SAVED_HUMAN_POSITION.x,
		SAVED_HUMAN_POSITION.y,
		SAVED_HUMAN_POSITION.orien_z,
		SAVED_HUMAN_POSITION.orien_w); //사람에게 가기.
									   // FUTURE: what if the person gone??

	//예전 알고리즘
	// do{
	// 	// 현재 로봇의 위치 받아오기
	//
	// }while(/*-이동한 거리가 사람의 반경 2m 이내가 아닐 경우까지 이동. -*/);
	// 	/*- 행동 중단 -*/
	// return;
	// //사람에게 가기.
	// do{
	// 	// 현재 로봇의 위치 받아오기
	// }while(/*-이동한 거리가 사람의 반경 2m 이내가 아닐 경우까지 이동. -*/);
	// 	/*- 행동 중단 -*/
	// return;
} // END


void bada_go_to_pepl_demo()
{
	// while
	bada_go_destination_blocking_demo(
		100.0,
		SAVED_HUMAN_POSITION.x,
		SAVED_HUMAN_POSITION.y,
		SAVED_HUMAN_POSITION.orien_z,
		SAVED_HUMAN_POSITION.orien_w); //사람에게 가기.
									   // FUTURE: what if the person gone??
} // END


void bada_aligned_pepl()
{
	ros::Rate loop_rate(10);
	ROS_INFO("Head up");
	bada_head_UP_cmd(true); // 2m 에 도달하면 카메라 위로 들기
	ros::Duration(0.1).sleep();
	bada_open_eyes_cmd(true);
	ros::Duration(0.1).sleep();

	float AngleV = 3.14f /10.0f;
	int cnt=0;
	while (ros::ok())
	{
		cnt++;
		loop_rate.sleep();
		ros::spinOnce();
		if(cnt%2==0){
			ROS_INFO("%f", PPL_ANGLE);
		}
		if (abs(PPL_ANGLE) < 90)
		{
			if (PPL_ANGLE < 0)
				bada_vel_cmd(0, AngleV); // CCW   Object is on Left side
			else
				bada_vel_cmd(0, -AngleV); // CW,   Object is on right side
		}
		if (abs(PPL_ANGLE) < (15 * PI / 180.0))
		{
			ROS_INFO("stop aligned pepl");
			bada_vel_cmd(0, 0);
			ros::Duration(0.2).sleep();
			bada_vel_cmd(0, 0);
			ros::Duration(0.1).sleep();
			break;
		}
	} //가운데로 맞추기
	bada_open_eyes_cmd(false);
	bada_head_UP_cmd(false); // 2m 에 도달하면 카메라 위로 들기
}

void bada_go_until_touch()
{ // 버튼 눌리기 전까지 전진하기
	geometry_msgs::Twist msg;
	msg.linear.x = 0.1;
	PPL_ANGLE=-90.0;

	ros::Rate loop_rate(10);
	ROS_INFO("Head up");
	bada_head_UP_cmd(false); // 2m 에 도달하면 카메라 위로 들기
	ros::Duration(0.1).sleep();
	bada_open_eyes_cmd(false);
	ros::Duration(0.1).sleep();

	float AngleV = 3.14f /10.0f;
	int cnt=0;

	do
	{
		pub_cmdvel.publish(msg); // 앞으로 전진
		ros::Duration(0.1).sleep();
		ros::spinOnce();


		loop_rate.sleep();
		// ros::spinOnce();
		// if(cnt%2==0){
		// 	ROS_INFO("%f", PPL_ANGLE);
		// }
		// if (abs(PPL_ANGLE) < 90)
		// {
		// 	if (PPL_ANGLE < 0)
				
		// 		bada_vel_cmd(0.1, AngleV); // CCW   Object is on Left side
		// 	else
		// 		bada_vel_cmd(0.1, -AngleV); // CW,   Object is on right side
		// }
	} while (ros::ok() && !SWITCH_CHECK);
	SWITCH_CHECK = 0;
	
	msg.linear.x = 0.0;
	pub_cmdvel.publish(msg); //스탑
} //END

void bada_change_pos(float LinePos, float AnglePos)
{
	// ros::Rate loop_rate(20);

	// int t = 0;
	// float vel = 0;
	// float target = (abs(LinePos) > abs(AnglePos)) ? LinePos : AnglePos;
	// do
	// {
	// 	t++;
	// 	if (abs(LinePos) > abs(AnglePos))
	// 	{
	// 		bada_vel_cmd(-0.15, 0);
	// 		vel = -0.15;
	// 	}
	// 	else
	// 	{
	// 		bada_vel_cmd(0, PI / 4.0f);
	// 		vel = PI / 4.0f;
	// 	}
	// 	loop_rate.sleep();
	// 	ros::spinOnce();

	// } while (ros::ok() && float(t) * 0.05 * abs(vel) <= abs(target));
	
	bada_vel_cmd(-0.1,0);
	waitSec(0.2);
	bada_vel_cmd(-0.1,0);
	waitSec(4);
	bada_vel_cmd(0,0);
	waitSec(0.2);
	bada_vel_cmd(0,0);
	
}

void sub_odometry_callback(const nav_msgs::Odometry &msg)
{
	// CURRENT_ROBOT_POSITION = msg;
}

// void bada_cancelAllGoal(){
// 	actionClient->cancelAllGoals();
// }

void bada_go_to_soundPT()
{ //사람 데리고 가는용
	bada_go_destination_blocking(100.0, SAVED_SOUND_POSITION.x, SAVED_SOUND_POSITION.y, SAVED_SOUND_POSITION.orien_z, SAVED_SOUND_POSITION.orien_w);
}
void bada_display_inform()
{
	bada_display_cmd(DISP_EVNT::CRYING_EVENT);
	waitSec(0.2);
	bada_display_cmd(DISP_EVNT::CRYING_EVENT);
	waitSec(2);
	bada_display_cmd(DISP_EVNT::NOTHING);
	waitSec(0.2);
	bada_display_cmd(DISP_EVNT::NOTHING);
}

void bada_emotion()
{
}

void bada_wait_button()
{	
	ROS_INFO("button in)");
	bool pressed = false;
	ros::Rate loop_rate(6);
	while (ros::ok() && !pressed)
	{
		ros::spinOnce();
		pressed = SWITCH_CHECK;
		loop_rate.sleep();
	}
	ROS_INFO("out!");

}

void bada_go_to_sound()
{ //소리나는 방향으로 이동
	// topic:
	//
	// - sub sound localization,
	//
	// - pub cmd vel
	// go to sound while tracking, stop when "enough"
	ros::Rate loop_rate(6); // 과부하방지로 멈추기
	int count = 0;
	while (ros::ok())
	{
		// move base go to : CURRENT_SOUND_DIRECTION
		// to align direction of the robot.
		// TODO:: cmd_vel or simple_goal
		bada_vel_cmd(0.3f, 0);
		count++;

		loop_rate.sleep(); // 6헤르츠가 적당할 듯. 연산 과부화 방지용.
		if (count > 30)
		{
			break;
		}
		ros::spinOnce();
	}
}

Position bada_get_robot_pos()
{
	tf::TransformListener listener;
	tf::StampedTransform transform;
	try
	{
		listener.waitForTransform("map","base_footprint_link",ros::Time(0),ros::Duration(3.0));
		listener.lookupTransform("map", "base_footprint_link", ros::Time(0), transform);
		CURRENT_ROBOT_POSITION.pose.pose.position.x = transform.getOrigin().x();
		CURRENT_ROBOT_POSITION.pose.pose.position.y = transform.getOrigin().y();
		CURRENT_ROBOT_POSITION.pose.pose.orientation.z = transform.getRotation().getZ();
		CURRENT_ROBOT_POSITION.pose.pose.orientation.w = transform.getRotation().getW();
	}
	catch (tf::TransformException ex)
	{
		ROS_ERROR("%s", ex.what());
		ROS_INFO("tf ERROR");
		ros::Duration(1.0).sleep();
	}

	Position pos = {CURRENT_ROBOT_POSITION.pose.pose.position.x, CURRENT_ROBOT_POSITION.pose.pose.position.y, CURRENT_ROBOT_POSITION.pose.pose.orientation.z, CURRENT_ROBOT_POSITION.pose.pose.orientation.w};
	return pos;
}

Position bada_get_robot_pos_demo()
{
	tf::TransformListener listener;
	tf::StampedTransform transform;
	try
	{
		listener.waitForTransform("odom","base_footprint_link",ros::Time(0),ros::Duration(3.0));
		listener.lookupTransform("odom", "base_footprint_link", ros::Time(0), transform);
		CURRENT_ROBOT_POSITION.pose.pose.position.x = transform.getOrigin().x();
		CURRENT_ROBOT_POSITION.pose.pose.position.y = transform.getOrigin().y();
		CURRENT_ROBOT_POSITION.pose.pose.orientation.z = transform.getRotation().getZ();
		CURRENT_ROBOT_POSITION.pose.pose.orientation.w = transform.getRotation().getW();
	}
	catch (tf::TransformException ex)
	{
		ROS_ERROR("%s", ex.what());
		ROS_INFO("tf ERROR");
		ros::Duration(1.0).sleep();
	}

	Position pos = {CURRENT_ROBOT_POSITION.pose.pose.position.x, CURRENT_ROBOT_POSITION.pose.pose.position.y, CURRENT_ROBOT_POSITION.pose.pose.orientation.z, CURRENT_ROBOT_POSITION.pose.pose.orientation.w};
	return pos;
}

void bada_save_sound_odom()
{
	Position pos = bada_get_robot_pos();
	SAVED_SOUND_POSITION = pos;
}

void bada_save_sound_odom_demo()
{
	Position pos = bada_get_robot_pos_demo();
	SAVED_SOUND_POSITION = pos;
}
//https://opentutorials.org/module/2894/16661

void bada_open_eyes_cmd(bool status)
{
	std_msgs::Bool BoolStatus;
	BoolStatus.data = status;
	pub_eyes_open.publish(BoolStatus);
	waitSec(0.5); // wait 0.5
} // True -> Eyes UP, False -> Eyes Down

void bada_head_UP_cmd(bool status)
{
	ROS_INFO("hamsu head");
	std_msgs::Bool BoolStatus;
	BoolStatus.data = status;
	pub_head_up.publish(BoolStatus);
	waitSec(1);
} // True -> UP, False -> DOWN

void bada_display_cmd(DISP_EVNT status)
{
	std_msgs::Int16 IntStatus;
	IntStatus.data = int(status);
	pub_display_cmd.publish(IntStatus);
}

void bada_vel_cmd(const float XLineVel, const float ZAngleVel)
{
	geometry_msgs::Twist msg;
	msg.linear.x = XLineVel;
	msg.angular.z = ZAngleVel;
	
	pub_cmdvel.publish(msg);
}

/*--------------------------------------Callback----------------------------------------------*/
void sub_pepl_checker_callback(const geometry_msgs::Point &msg)
{
	PPL_CHECK = true;
	PPL_ANGLE = msg.y; // [rad]
	ROS_INFO("PPL_CHECK");
}

void sub_switch_checker_callback(const std_msgs::Int16 &msgs)
{
	SWITCH_CHECK = msgs.data;
}

void sub_signal_callback(const std_msgs::String &msg)
{
	SIGNAL = msg;
}

void sub_sound_localization_callback(const geometry_msgs::PoseStamped &msg)
{
	CURRENT_SOUND_DIRECTION = msg;
}

bool bada_go_to_sound2()
{ //로봇 base_link 기준, 소리 나는 방향보고 1m 전진
	actionClient->waitForServer();
	ROS_INFO("going to sound");
	move_base_msgs::MoveBaseGoal sound_goal;
	move_base_msgs::MoveBaseGoal sound_goal2;

	sound_goal.target_pose.header.frame_id = "base_footprint_link";
	sound_goal.target_pose.header.stamp = ros::Time::now();

	sound_goal.target_pose.pose.position.x = 0;
	sound_goal.target_pose.pose.orientation.z = CURRENT_SOUND_DIRECTION.pose.orientation.z;
	sound_goal.target_pose.pose.orientation.w = CURRENT_SOUND_DIRECTION.pose.orientation.w;
	actionClient->sendGoal(sound_goal);
	actionClient->waitForResult(ros::Duration(30.0));

	sound_goal2.target_pose.header.frame_id = "base_footprint_link";
	sound_goal2.target_pose.header.stamp = ros::Time::now();

	sound_goal2.target_pose.pose.position.x = 1.0;
	sound_goal2.target_pose.pose.orientation.w = 1.0;
	actionClient->sendGoal(sound_goal2);

	bool finished_before_timeout = actionClient->waitForResult(ros::Duration(30.0));
	ROS_INFO("go to sound done.");

	if (finished_before_timeout)
	{
		actionlib::SimpleClientGoalState state = actionClient->getState();
		ROS_INFO("go to sound Action finished: %s", state.toString().c_str());
	}
	else
		ROS_INFO("go to sound Action did not finish before the time out.");

	if (SIGNAL.data == LastSignal.data)
		return true;
	else
		return false;
}

void waitSec(float sec)
{
	ros::Duration(sec).sleep();
}

void bada_log(std::string str1)
{
	// StringMsg pubData;
	// pubData.data=msg;
	// pub_logger.publish(pubData);

	std_msgs::String msg;

	std::stringstream ss;
	ss << str1;
	msg.data = ss.str();

	ROS_INFO("%s", msg.data.c_str());

	/**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
	pub_logger.publish(msg);

	// ros::spinOnce();
}
