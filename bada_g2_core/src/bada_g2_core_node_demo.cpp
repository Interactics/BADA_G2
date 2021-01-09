#include <ros/ros.h>
#include "bada_g2_core.h"

#define DEMO -1

int main(int argc, char **argv)
{
	STATE state 		= FINDING_PEPL;
	ros::init(argc, argv, "bada_g2_core");
	ROS_INFO("starting bada_g2_core... 06010352");

	ros::NodeHandle nh;
	pub_cmdvel      	= nh.advertise<geometry_msgs::Twist>("/bada/cmd_vel", 1);
	pub_eyes_open   	= nh.advertise<std_msgs::Bool>("/bada/eyes/open", 1);
	pub_head_up    		= nh.advertise<std_msgs::Bool>("/sensor/DXup", 1);
	pub_display_cmd 	= nh.advertise<std_msgs::Int16>("/sensor/Display", 1);
	pub_logger 			= nh.advertise<std_msgs::String>("/bada/log", 1);
	// pub_pose = nh.advertise<geometry_msgs::PoseStamped>("/bada/pose", 1);

	// sub_odometry           = nh.subscribe("/bada/odom", 1, sub_odometry_callback);
	sub_pepl_checker	= nh.subscribe("/bada/eyes/distance", 1, sub_pepl_checker_callback); //TODO: FIX CALLBACK FUNCTION
	sub_sig_checker 	= nh.subscribe("/bada/audio/checker", 1, sub_sig_checker_callback);
	sub_signal 			= nh.subscribe("/bada/audio/signal", 1, sub_signal_callback);
	sub_switch_checker 	= nh.subscribe("/sensor/button", 1, sub_switch_checker_callback);
	sub_sound_localization = nh.subscribe("/bada/audio/localization_filtered", 1, sub_sound_localization_callback);

	bada_log("bada started");

	actionClient 		= new MoveBaseClient("move_base", true); //move_base client 선언

	bool is_there_pepl 	= false;
	bool is_sound_same 	= false;

	ros::Rate loop_rate(20);
	int current = 0;
	int target;
			// bool finding_pepl_fail = false;
	//bada_aligned_pepl(); // 사람의 위치고 로봇 사람을 가운데로
	waitSec(2);
	// bada_aligned_pepl(); // 사람의 위치고 로봇 사람을 가운데로

	// ROS_INFO("align ppl done");
	// bada_go_until_touch(); // 버튼 눌리기 전까지 전진하기
	// ROS_INFO("touched. moving back.");
	// bada_change_pos(-0.3, 0); // 뒤로 1m 이동

	// //bada_change_pos(0, PI); // 180도 회전
	// ROS_INFO("change pos done");

	// while(ros::ok())
	// {
	// 	ros::spinOnce();

	// 	loop_rate.sleep();
			
	// }
	
	// bada_roaming();
	// return 0;
	bada_display_cmd(DISP_EVNT::LOGO);
	waitSec(3);
	bada_display_cmd(DISP_EVNT::NOTHING);
	waitSec(1);
	bada_display_cmd(DISP_EVNT::NOTHING);
	waitSec(1);

#ifdef DEMO
	while (ros::ok())
	{
		switch (state)
		{
		case FINDING_PEPL: //turn & findpeople
			ROS_INFO("starting FINDING_PEOPLE");
			do
			{
				target = current % 3;
				// TODO: wait until action client success
				bada_go_destination_blocking(80.0, wayPoint[target][0], wayPoint[target][1], wayPoint[target][2], wayPoint[target][3]); // Go to POINT of ROOM
				bada_head_UP_cmd(true);																									// HEAD_UP
				is_there_pepl = bada_rounding();
				bada_head_UP_cmd(false); // HEAD_DOWN
				if (is_there_pepl)
				{	ROS_INFO("break");
					break;
				}
				current++;
			} while (ros::ok());
			bada_next_state(state);
			break;
		case ROAMING:
			ROS_INFO("starting ROAMING");
			// bada_display_cmd(DISP_EVNT::A_UP);
			// waitSec(1);
			// bada_display_cmd(DISP_EVNT::NOTHING);
			bada_roaming(); // 카테고리에 등록된 소리가 나올 때까지 배회하기
			waitSec(1);
			// waitSec(1);
			// bada_display_cmd(DISP_EVNT::EXCLAMATION);
			// waitSec(1);
			// bada_display_cmd(DISP_EVNT::NOTHING);
			// waitSec(1);
			// bada_display_cmd(DISP_EVNT::EXCLAMATION);
			// waitSec(1);
			// bada_display_cmd(DISP_EVNT::NOTHING);
			ROS_INFO("roaming done");
			LastSignal = SIGNAL;
			// if(LastSignal.data == "Cry"){
			// 	bada_display_cmd(DISP_EVNT::CRYING_EVENT);
			// } else if(LastSignal.data == "Alarm") {
			// 	bada_display_cmd(DISP_EVNT::FIRE_EVENT);
			// }
			waitSec(1);
			bada_display_cmd(DISP_EVNT::NOTHING);
			waitSec(0.2);
			bada_display_cmd(DISP_EVNT::NOTHING);
			/////save souud pos
			bada_save_sound_odom();

			bada_next_state(state);	
			break;
		case SOUND_DETECTING:
			ROS_INFO("pass go to sound");
			bada_next_state(state);
			break;

			// ROS_INFO("starting SOUND_DETECTING");
			// // 소리 난 방향 서브스크라이빙하기
			// // 소리 발생한 방향으로 이동하기
			// // 저장하기
			// // TODO:
			// is_sound_same = bada_go_to_sound2(); //소리 발생하는 방향으로 충분히 이동하기.

			// if (!is_sound_same)
			// {
			// 	ROS_INFO("sound detecting different");
			// 	bada_set_state(state, STATE::ROAMING);
			// 	break;
			// }
			// bada_save_sound_odom();
			// // bada_save_sound_PT();					//로봇의 현재 위치와 소리나는 방향 저장하기.
			// bada_next_state(state);
			// ROS_INFO("sound detecting same");
			// break;
		case MOVING_TO_PEPL:
			bada_display_cmd(DISP_EVNT::EXCLAMATION);
			waitSec(1);
			bada_display_cmd(DISP_EVNT::NOTHING);
			ROS_INFO("starting MOVING_TO_PEPL");
			waitSec(1);
			bada_go_to_pepl(); // 반경 2m 이내 도달 검사하기. 그렇지 않으면 계속 접근
			ROS_INFO("go to ppl done");
				// ////사람이 없으면
				// is_there_pepl = bada_rounding();
				// if (!is_there_pepl)
				// {
				// 	state = FINDING_PEPL;
				// 	finding_pepl_fail = true;
				// 	//move_to_finding_pepl이 true면 state1에서 다시 aligned로 돌아가야함;
				// 	break;
				// }
			waitSec(1);
			bada_aligned_pepl(); // 사람의 위치고 로봇 사람을 가운데로

			ROS_INFO("align ppl done");
			bada_go_until_touch(); // 버튼 눌리기 전까지 전진하기
			ROS_INFO("touched. moving back.");
			bada_change_pos(-0.3, 0); // 뒤로 1m 이동

			//bada_change_pos(0, PI); // 180도 회전
			ROS_INFO("change pos done");
			bada_next_state(state);
			break;
		case MOVING_WITH_PEPL:
			bada_display_cmd(DISP_EVNT::EXCLAMATION);
			waitSec(1);
			bada_display_cmd(DISP_EVNT::NOTHING);
			//waitSec(1);
			
			ROS_INFO("starting MOVING_WITH_PEPL");
			waitSec(1);
			ROS_INFO("waiting for button input");
			bada_wait_button(); // 버튼 눌리기 전까지 대기하기.
			if(LastSignal=="Cry"){
				bada_display_cmd(DISP_EVNT::CRYING_EVENT);
			} else if(LastSignal=="Alarm") {
				bada_display_cmd(DISP_EVNT::FIRE_EVENT);
			} else if(LastSignal=="Door") {
				bada_display_cmd(DISP_EVNT::DOOR_EVENT);
			} else if(LastSignal=="Water") {
				bada_display_cmd(DISP_EVNT::WATER_EVENT);
			} else if(LastSignal=="Bell") {
				bada_display_cmd(DISP_EVNT::BELL_EVENT);
			} else {
				ROS_INFO("ERROR");
				bada_display_cmd(DISP_EVNT::LOGO);
			}
			ROS_INFO("wait button done");
			bada_go_to_soundPT(); // 소리가 발생한 지점으로 이동하기.
			ROS_INFO("go to sound pt done");
			bada_display_inform(); // 해당 지점에서 소리

			bada_emotion();		   // 완료함 표현하기.
			ROS_INFO("finished");
			//bada_next_state(state);
			return 0;
			break;
		} // END_SWITCH
	}	  // END_WHILE

#endif

#ifndef DEMO
		while (ros::ok())
		{
			switch (state)
			{
			case FINDING_PEPL: //turn & findpeople
				ROS_INFO("starting FINDING_PEOPLE");
				do
				{
					target = current % 3;
					// TODO: wait until action client success
					bada_go_destination_blocking(80.0, wayPoint[target][0], wayPoint[target][1], wayPoint[target][2], wayPoint[target][3]); // Go to POINT of ROOM
					bada_head_UP_cmd(true);																									// HEAD_UP
					is_there_pepl = bada_rounding();
					bada_head_UP_cmd(false); // HEAD_DOWN
					if (is_there_pepl)
					{
						ROS_INFO("break");
						break;
					}
					current++;
				} while (ros::ok());
				bada_next_state(state);
				break;
			case ROAMING:
				ROS_INFO("starting ROAMING");
				// bada_display_cmd(DISP_EVNT::A_UP);
				// waitSec(1);
				// bada_display_cmd(DISP_EVNT::NOTHING);
				bada_roaming(); // 카테고리에 등록된 소리가 나올 때까지 배회하기
				waitSec(1);
				// waitSec(1);
				// bada_display_cmd(DISP_EVNT::EXCLAMATION);
				// waitSec(1);
				// bada_display_cmd(DISP_EVNT::NOTHING);
				// waitSec(1);
				// bada_display_cmd(DISP_EVNT::EXCLAMATION);
				// waitSec(1);
				// bada_display_cmd(DISP_EVNT::NOTHING);
				ROS_INFO("roaming done");
				LastSignal = SIGNAL;
				// if(LastSignal.data == "Cry"){
				// 	bada_display_cmd(DISP_EVNT::CRYING_EVENT);
				// } else if(LastSignal.data == "Alarm") {
				// 	bada_display_cmd(DISP_EVNT::FIRE_EVENT);
				// }
				waitSec(1);
				bada_display_cmd(DISP_EVNT::NOTHING);
				waitSec(0.2);
				bada_display_cmd(DISP_EVNT::NOTHING);
				/////save souud pos
				bada_save_sound_odom();

				bada_next_state(state);
				break;
			case SOUND_DETECTING:
				ROS_INFO("pass go to sound");
				bada_next_state(state);
				break;

				// ROS_INFO("starting SOUND_DETECTING");
				// // 소리 난 방향 서브스크라이빙하기
				// // 소리 발생한 방향으로 이동하기
				// // 저장하기
				// // TODO:
				// is_sound_same = bada_go_to_sound2(); //소리 발생하는 방향으로 충분히 이동하기.

				// if (!is_sound_same)
				// {
				// 	ROS_INFO("sound detecting different");
				// 	bada_set_state(state, STATE::ROAMING);
				// 	break;
				// }
				// bada_save_sound_odom();
				// // bada_save_sound_PT();					//로봇의 현재 위치와 소리나는 방향 저장하기.
				// bada_next_state(state);
				// ROS_INFO("sound detecting same");
				// break;
			case MOVING_TO_PEPL:
				bada_display_cmd(DISP_EVNT::EXCLAMATION);
				waitSec(1);
				bada_display_cmd(DISP_EVNT::NOTHING);
				ROS_INFO("starting MOVING_TO_PEPL");
				waitSec(1);
				bada_go_to_pepl(); // 반경 2m 이내 도달 검사하기. 그렇지 않으면 계속 접근
				ROS_INFO("go to ppl done");
				// ////사람이 없으면
				// is_there_pepl = bada_rounding();
				// if (!is_there_pepl)
				// {
				// 	state = FINDING_PEPL;
				// 	finding_pepl_fail = true;
				// 	//move_to_finding_pepl이 true면 state1에서 다시 aligned로 돌아가야함;
				// 	break;
				// }
			waitSec(1);
			bada_aligned_pepl(); // 사람의 위치고 로봇 사람을 가운데로

			ROS_INFO("align ppl done");
			bada_go_until_touch(); // 버튼 눌리기 전까지 전진하기
			ROS_INFO("touched. moving back.");
			bada_change_pos(-0.3, 0); // 뒤로 1m 이동

			//bada_change_pos(0, PI); // 180도 회전
			ROS_INFO("change pos done");
			bada_next_state(state);
			break;
		case MOVING_WITH_PEPL:
			bada_display_cmd(DISP_EVNT::EXCLAMATION);
			waitSec(1);
			bada_display_cmd(DISP_EVNT::NOTHING);
			//waitSec(1);
			
			ROS_INFO("starting MOVING_WITH_PEPL");
			waitSec(1);
			ROS_INFO("waiting for button input");
			bada_wait_button(); // 버튼 눌리기 전까지 대기하기.
			if(LastSignal=="Cry"){
				bada_display_cmd(DISP_EVNT::CRYING_EVENT);
			} else if(LastSignal=="Alarm") {
				bada_display_cmd(DISP_EVNT::FIRE_EVENT);
			} else if(LastSignal=="Door") {
				bada_display_cmd(DISP_EVNT::DOOR_EVENT);
			} else if(LastSignal=="Water") {
				bada_display_cmd(DISP_EVNT::WATER_EVENT);
			} else if(LastSignal=="Bell") {
				bada_display_cmd(DISP_EVNT::BELL_EVENT);
			} else {
				ROS_INFO("ERROR");
				bada_display_cmd(DISP_EVNT::LOGO);
			}
			ROS_INFO("wait button done");
			bada_go_to_soundPT(); // 소리가 발생한 지점으로 이동하기.
			ROS_INFO("go to sound pt done");
			bada_display_inform(); // 해당 지점에서 소리

			bada_emotion();		   // 완료함 표현하기.
			ROS_INFO("finished");
			//bada_next_state(state);
			return 0;
			break;
		} // END_SWITCH
		ros::spinOnce();
	} // END_WHILE
#endif

} // END_MAIN
