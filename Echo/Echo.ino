#include <ros.h>
#include <std_msgs/String.h>

std_msgs::String str_msg;

ros::NodeHandle nh;
ros::Publisher pub("echoed_words", &str_msg);

void echoMessage(const std_msgs::String& yelled_words)
{
  String YW;
  char* EW;
  std_msgs::String echoed_words;

  //convert input data to string to allow for string operations
  YW = String(yelled_words.data);

  //Store echoed words in a char array
  (YW + "..." + YW + "...").toCharArray(EW, 100);

  
  echoed_words.data = EW;
  pub.publish( &echoed_words );
  nh.spinOnce();
}
ros::Subscriber<std_msgs::String> sub("yelled_words", &echoMessage);

void setup() 
{
  nh.subscribe(sub);
  nh.advertise(pub);
}

void loop() 
{
  // put your main code here, to run repeatedly:

}
