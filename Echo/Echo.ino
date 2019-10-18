#include <ros.h>
#include <std_msgs/String.h>

std_msgs::String str_msg;

ros::NodeHandle nh;
ros::Publisher pub("echoed_words", &str_msg);

void echoMessage(const std_msgs::String& yelled_words)
{
  const char* dots = "...";

  char* echo_char = yelled_words.data;
  String yell = String(yelled_words.data);
  const char* yell_char = yell.c_str();
  
  strcat(echo_char, dots);
  strcat(echo_char, yell_char);
  strcat(echo_char, dots);
  strcat(echo_char, yell_char);
  strcat(echo_char, dots);

  std_msgs::String echoed_words;
  echoed_words.data = echo_char;
  pub.publish( &echoed_words );
}


/*
void echoMessage(const std_msgs::String & yelled_words)
{
  pub.publish(&yelled_words);
}
*/

ros::Subscriber<std_msgs::String> sub("yelled_words", &echoMessage);

void setup() 
{
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub);
}

void loop() 
{
  //str_msg.data = "hello, world";
  //pub.publish( &str_msg );
  nh.spinOnce();
  delay(50);
  // put your main code here, to run repeatedly:

}
