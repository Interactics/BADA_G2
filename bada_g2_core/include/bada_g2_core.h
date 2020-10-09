#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_listener.h>
#include <math.h>

const double PI{3.141592};

//---------------- 노드 변경할 것 ----------------------------------------
/*
	TODO
	3. 각 상태마다 디스플레이에 이미지 표현하도록. 변경
	*/
//================================================================================

enum STATE
{
	FINDING_PEPL,
	ROAMING,
	SOUND_DETECTING,
	MOVING_TO_PEPL,
	MOVING_WITH_PEPL
};

enum DISP_EVNT
{
	NOTHING = 0,
	A_UP,
	A_DOWN,
	A_LEFT,
	A_RIGHT,
	FIRE_EVENT,
	WATER_EVENT,
	DOOR_EVENT,
	BELL_EVENT,
	BOILING_EVENT,
	CRYING_EVENT
};

struct Position
{
	double x;
	double y;
	double orien_z;
	double orien_w;
};

typedef std_msgs::Bool BoolMsg;
typedef std_msgs::String StringMsg;

std_msgs::Bool HEAD_STATUS;
std_msgs::Int16 MAT_STATUS;

void bada_set_state(STATE &present_state, STATE target_state);
void bada_next_state(STATE &present_state);
void bada_roaming();
void bada_send_destination(double x, double y, double orien_z, double orien_w); //맵 위치 x,y quaternion z,w 를 설정해주고 그 위치로 이동							// 배회하나 소리가 나면 다음으로 넘어간다.
void bada_go_destination_blocking(double duration, double x, double y, double orien_z, double orien_w);
void bada_save_current_position();					 // calculate the person's position on map from robot position using detected angle and theta.
bool bada_rounding();								 // 회전하며 사람이 있는지를 검사한다.
void bada_head_UP_cmd(bool STATUS);					 // 카메라달린 모터 위로 들기 for 사람 위치 확인용
void getCurrentRobotPositionTODO();					 // get current transform position(pose, quaternion) of robot
void bada_change_pos(float LinePos, float AnglePos); // 로봇에게 직선거리 혹은 회전 명령 주기
													 // 특정 위치만큼만 이동하기.
													 /* 리니어, 앵귤러에 도달할 때까지 회전하도록하기. 기본 속도는 정해져있다. 
													   보내는 것은 cmd_vel, 받는 것은 오도메트리 정보. */
void bada_aligned_pepl();							 // 사람 찾고 로봇과 사람 위치 정렬하기.
void go_until_touch();								 // 버튼이 눌릴 때까지 전진.
void bada_go_to_pepl();
void bada_go_until_touch();
void bada_save_sound_PT(); //로봇의 현재 위치와 소리나는 방향 저장하기.
void bada_go_to_soundPT(); // 소리가 발생한 지점으로 이동하기.
void bada_display_inform();
void bada_emotion();

void bada_wait_button();

void bada_save_sound_odom();
void bada_go_to_sound(); //소리 발생하는 방향으로 충분히 이동하기.
Position bada_get_robot_pos();

void bada_open_eyes_cmd(bool Status);									// Open Eyes Function.
void bada_display_cmd(DISP_EVNT status);								// Display Command
void bada_vel_cmd(const float XLineVel = 0, const float ZAngleVel = 0); // commendation of Publishing Velocity
void bada_log(std::string str1);										// commendation of Publishing Velocity
bool bada_go_to_sound2();												//로봇 base_link 기준, 소리 나는 방향보고 1m 전진

void waitSec(float sec);

ros::Publisher pub_cmdvel;
ros::Publisher pub_camera;
ros::Publisher pub_eyes_open;
ros::Publisher pub_head_up;
ros::Publisher pub_display_cmd;
ros::Publisher pub_logger;
ros::Publisher pub_pose;

ros::Subscriber sub_odometry;
ros::Subscriber sub_pepl_checker;
ros::Subscriber sub_sig_checker;
ros::Subscriber sub_switch_checker;
ros::Subscriber sub_signal;
ros::Subscriber sub_sound_localization;

/*--------------------------------------Callback----------------------------------------------*/

void sub_pepl_checker_callback(const geometry_msgs::Point &msg);
void sub_odometry_callback(const nav_msgs::Odometry &msg);
void sub_sig_checker_callback(const std_msgs::Empty &msg);					 // Roaming 단계에서 사용. 소리가 발생할 경우에 쓸모가 있다.
void sub_switch_checker_callback(const std_msgs::Bool &msgs);				 // Roaming 단계에서 사용. 소리가 발생할 경우에 쓸모가 있다.
void sub_signal_callback(const std_msgs::String &msg);						 //
void sub_sound_localization_callback(const geometry_msgs::PoseStamped &msg); //

//==============================================================================================

//사람이 일정 ROI에 들어오는 것을 검사함. 만약 ROI에 들어온다면, Checker는 True로 바뀜.

bool SIG_CHECK = false;	   // Roaming 단계에서 사용.
bool SWITCH_CHECK = false; // is Switch on?  T/F
bool PPL_CHECK = false;	   // Is there PPL?
float PPL_ANGLE = -90;	   // Angle of PPL respect to camera
float PPL_DIST = -1.0;	   // Distnace to PPL from

int CURRENT_POINT = 0;		//ROAMING CURRNET POINT

//geometry_msgs::Pose2D PERSON_POSITION;
nav_msgs::Odometry CURRENT_ROBOT_POSITION;
geometry_msgs::PoseStamped CURRENT_SOUND_DIRECTION;
std_msgs::String SIGNAL;
std_msgs::String LastSignal;

Position SAVED_SOUND_POSITION = {0.0f, 1.0f, 2.0f, 3.0f};
Position SAVED_HUMAN_POSITION = {0, 1, 2, 3};

double wayPoint[][4] = {
	{3.293, 1.023, 0.028, 1.000}, //way1
	{-0.097, 0.548, 1.000, -0.006},	//way2
	{-0.854, -1.400, -0.716, 0.699}	//way3
};									//roaming 장소 저장

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
MoveBaseClient *actionClient;

// sound position
// aligning: cam up
// button - twice?
// 