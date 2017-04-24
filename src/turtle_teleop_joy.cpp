#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

  ros::Publisher vel_pub_;
  geometry_msgs::Twist twist;
  bool teleoperated = true;
class TeleopTurtle
{
public:
  TeleopTurtle();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  int linear_, angular_;
  double l_scale_, a_scale_;
  double speed, button;
  ros::Subscriber joy_sub_;

private:
  void cmdCallback(const geometry_msgs::Twist::ConstPtr& cmd);

  ros::NodeHandle nh;

  //int linear_, angular_;
  //double l_scale_, a_scale_;
  //double speed, button;
  ros::Subscriber cmd_sub_;

};



TeleopTurtle::TeleopTurtle():
  linear_(1),
  angular_(2)
{

  nh_.param("axis_linear", linear_, linear_);
  nh_.param("axis_angular", angular_, angular_);
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);


  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);

  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopTurtle::joyCallback, this);
  cmd_sub_ = nh.subscribe<geometry_msgs::Twist>("cmd", 10, &TeleopTurtle::cmdCallback, this);
}


void TeleopTurtle::cmdCallback(const geometry_msgs::Twist::ConstPtr& cmd)
{

  if (teleoperated == false) twist=*cmd;
	//twist.angular.z = a_scale_*joy->axes[angular_];
	//twist.linear.x += l_scale_*joy->axes[linear_]/10.0;
	//if (joy->buttons[5] == 1) twist.angular.z = twist.linear.x = 0;
	//twist.angular.y = 0.0;
	//if (joy->buttons[3] == 1) twist.angular.y = 0.1;
        //if (joy->buttons[1] == 1) twist.angular.y = -0.1;

        //ROS_INFO( "Rychlost z= %f, Rychlost x= %f ",  twist.linear.z,twist.lineard);
}
void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{      
 	if (teleoperated != joy->buttons[4])  {twist.angular.z = twist.linear.x = 0;
        twist.angular.y = 0.0;}	
        teleoperated = (joy->buttons[4] == 1);
        if (teleoperated == true){
	twist.angular.z = a_scale_*joy->axes[angular_];
        twist.linear.x += l_scale_*joy->axes[linear_]/10.0;
        if (joy->buttons[5] == 1) twist.angular.z = twist.linear.x = 0;
        twist.angular.y = 0.0;
        if (joy->buttons[3] == 1) twist.angular.y = 0.1;
	if (joy->buttons[1] == 1) twist.angular.y = -0.1;

        ROS_INFO( "Rychlost z= %f, Rychlost x= %f ",  twist.linear.z,twist.linear.x);
}}

 main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_turtle");
  TeleopTurtle teleop_turtle;
  while (ros::ok()){
	  ros::spinOnce();
	  vel_pub_.publish(twist);
	  usleep(50000);
  }
}
