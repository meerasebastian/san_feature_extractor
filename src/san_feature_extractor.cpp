#include <ros/ros.h>

#include <std_msgs/String.h>

#include <people_msgs/PositionMeasurement.h>
#include <people_msgs/PositionMeasurementArray.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <sensor_msgs/LaserScan.h>
#include <visualization_msgs/Marker.h>
#include<geometry_msgs/Point32.h>

#include<san_feature_extractor/peopleData.h>
#include<san_feature_extractor/peopleHallwayFeatures.h>
#include<hallway/hallwayMsg.h>

#include<string.h>
#include<math.h>
#include<fstream>
#include<ctime>


using namespace std;

struct personDetails{
  string frame_id;
  string  personId;
  float distanceFromPR2;
  float distance_travelled;
  double posx, posy;
  double t0, t1, speed, xdistance, ydistance;
}peopleData[100]; 

struct hallwayDetails{
  float  m; //Slope of Left Hallway
  float c; //Intercept of Left Hallway
  float width; //Width of the Hallway
}hallwayData;

string peoplename[100];
int personFound = 0

int noOfPeople = 0, flag = 0, trajectory = 0, peopleFlag = 0, trajectoryPeopleFlag = 0, firstPersonNo = 0, tracetoryFirstPersonNo = 0;
float robotPositionX = 0, robotPositionY = 0;
double currentTime, startTime;
double trajectoryCurrentTime, trajectoryStartTime;
//time_t now = time(0);
//char* dt = ctime(&now);
//strcat(dt,".txt");
ofstream outfile ("Dataset1.txt");
ofstream testfile ("Testdataset1.txt");

ofstream trajectoryOutfile ("TrajectoryDataset1.txt");
ofstream trajectoryTestfile ("TrajectoryTestdataset1.txt");

ros::Publisher people_features, hallway_pub;
san_feature_extractor::peopleData personDetails;

geometry_msgs::Point robot_pose;

string Convert (float number){
  ostringstream buff;
  buff<<number;
  return buff.str();   
}

void peoplePositionCallback(const people_msgs::PositionMeasurementArray::ConstPtr& msg){
  for( int i = 0; i < msg->people.size(); i++ ){
    if(personFound == 0){
      
    }
  }
}

/*void peoplePositionCallback(const people_msgs::PositionMeasurementArray::ConstPtr& msg){
  
  int newPersonFound = 0;
  float currentDistance;
  std::vector<people_msgs::PositionMeasurement> people;
  
  for( int i = 0; i < msg->people.size(); i++ ){
    for( int j = 0; j < noOfPeople; j++){
      newPersonFound = 0;
      
      //Tracking the person
      if( strcmp(msg->people[i].name.c_str() , peopleData[j].personId.c_str()) == 0 ) {
        
        if(peopleData[j].xdistance!=msg->people[i].pos.x && peopleData[j].ydistance!=msg->people[i].pos.y){
          //ROS_INFO("\n\tPerson Details ");
          //ROS_INFO("Person Id : %s", peopleData[j].personId.c_str());
          //ROS_INFO("Previous Distance : %f", sqrt(pow(peopleData[j].xdistance,2) + pow(peopleData[j].ydistance,2)));
          
          peopleData[j].xdistance = 100 * msg->people[i].pos.x; //Converting m to cm
          peopleData[j].ydistance = 100 * msg->people[i].pos.y; //Converting m to cm
          peopleData[j].posx =  100 * msg->people[i].pos.x; //Converting m to cm
          peopleData[j].posy = 100 * msg->people[i].pos.y; //Converting m to cm
          currentDistance = sqrt(pow((peopleData[j].xdistance - robotPositionX),2)+pow((peopleData[j].ydistance - robotPositionY),2));
          peopleData[j].distance_travelled =  abs(currentDistance - peopleData[j].distanceFromPR2);
          peopleData[j].distanceFromPR2 = currentDistance;
          peopleData[j].t0 =  peopleData[j].t1; 
          peopleData[j].t1 = ros::Time::now().toSec();
          peopleData[j].speed =  peopleData[j].distance_travelled / ( peopleData[j].t1 -  peopleData[j].t0);
          
          //ROS_INFO("Time T0 : %f", peopleData[j].t0);
          //ROS_INFO("Time T1 : %f", peopleData[j].t1);
          //ROS_INFO("Speed : %f", peopleData[j].speed);
          //ROS_INFO("Distance travelled : %f", peopleData[j].distance_travelled);
          
          //Publish the details
          personDetails.frame_id = "base_link";
          personDetails.personId = peopleData[j].personId; 
          personDetails.distanceFromPR2 = peopleData[j].distanceFromPR2;
          personDetails.x = peopleData[j].posx;
          personDetails.y = peopleData[j].posy;
          personDetails.t0 = peopleData[j].t0; 
          personDetails.t1 = peopleData[j].t1;
          personDetails.xdistance = peopleData[j].xdistance;
          personDetails.ydistance = peopleData[j].ydistance;
          personDetails.distance_travelled = peopleData[j].distance_travelled; 
          personDetails.speed = peopleData[j].speed;
          people_features.publish(personDetails);  
          
          newPersonFound = 1;
          break;  
        }
      }
    }
    
    //Push the details of new person
    if(newPersonFound == 0){
      peopleData[noOfPeople].frame_id = "base_link";
      //Extract Person Id(Number) from Person Name(String)
      //char* personId = msg->people[i].name.c_str();
      //int IdLength = personId.strlen();
      
      peopleData[noOfPeople].personId = msg->people[i].name.c_str();
      peopleData[noOfPeople].t0 = ros::Time::now().toSec();
      peopleData[noOfPeople].t1 = ros::Time::now().toSec();
      peopleData[noOfPeople].xdistance = 100 * msg->people[i].pos.x; //Converting m to cm
      peopleData[noOfPeople].ydistance = 100 * msg->people[i].pos.y; //Converting m to cm
      peopleData[noOfPeople].posx =  100 * msg->people[i].pos.x; //Converting m to cm
      peopleData[noOfPeople].posy = 100 * msg->people[i].pos.y; //Converting m to cm
      peopleData[noOfPeople].distanceFromPR2 = sqrt(pow((peopleData[noOfPeople].xdistance - robotPositionX),2) + pow((peopleData[noOfPeople].ydistance - robotPositionY),2)); //2-D distance
      peopleData[noOfPeople].distance_travelled = 0.0;
      peopleData[noOfPeople].speed = 0.0;
      
      //Publish the details
      personDetails.frame_id = "base_link";
      personDetails.personId = peopleData[noOfPeople].personId; 
      personDetails.distanceFromPR2 = peopleData[noOfPeople].distanceFromPR2;
      personDetails.t0 = peopleData[noOfPeople].t0; 
      personDetails.t1 = peopleData[noOfPeople].t1;
      personDetails.x = peopleData[noOfPeople].posx;
      personDetails.y = peopleData[noOfPeople].posy;
      personDetails.xdistance = peopleData[noOfPeople].xdistance;
      personDetails.ydistance = peopleData[noOfPeople].ydistance;
      personDetails.distance_travelled = peopleData[noOfPeople].distance_travelled; 
      personDetails.speed = peopleData[noOfPeople].speed;
      people_features.publish(personDetails);
      
      //ROS_INFO("New Person to the structure : %s", peopleData[noOfPeople].personId.c_str());
      //ROS_INFO("\n\tPerson Details ");
      //ROS_INFO("Person Id : %s added", peopleData[noOfPeople].personId.c_str());
      //ROS_INFO("Position : %f    %f", peopleData[noOfPeople].xdistance,peopleData[noOfPeople].ydistance);
      //ROS_INFO("Distance from pr2: %f", peopleData[noOfPeople].distanceFromPR2);
      //ROS_INFO("Time T0 , T1  : %f , %f", peopleData[noOfPeople].t0, peopleData[noOfPeople].t0);
       
      noOfPeople = noOfPeople + 1;
    }
    
  }
  
}*/

void hallwayDetectionCallback(const hallway::hallwayMsg::ConstPtr& msg){
  float timeStamp;
  string time_Stamp;
  string p1ID, p2ID;
  int i;
  float p1HallwayL, p1HallwayR, p2HallwayL, p2HallwayR, p1p2Distance; 
  string p1_HallwayL, p1_HallwayR, p2_HallwayL, p2_HallwayR, p1p2_Distance; 
  
  string p1DistanceFromPR2, p2DistanceFromPR2;
  
  san_feature_extractor::peopleHallwayFeatures hallwayRelatedFeatures;
  float m,c, width;

  m = msg->slope_hallwayL; //Slope of Left Hallway
  c = msg->intercept_hallwayL; //Intercept of Left Hallway
  width = msg->width_hallway; //Width of the Hallway
  
  hallwayData.m = m;
  hallwayData.c = c;
  hallwayData.width = width;
  
  for( int index = 0; index < 1; index++){
    hallwayRelatedFeatures.frame_id = "base_link";
    p1ID = peopleData[index].personId.c_str();  
    p1HallwayR = ( abs( m*peopleData[index].xdistance - peopleData[index].ydistance + c ) ) / ( sqrt( pow(m,2) + 1 ) );
    p1HallwayL = msg->width_hallway - p1HallwayR;
    
    hallwayRelatedFeatures.personId = p1ID;
    hallwayRelatedFeatures.distancefromHallwayR = p1HallwayR;
    hallwayRelatedFeatures.distancefromHallwayL = p1HallwayL;
    hallway_pub.publish(hallwayRelatedFeatures);
    
    //Details of first pperson - to test the distanve betwee person1 and hallway
    /*ROS_INFO("\n\tHallway Features");
    ROS_INFO("Person id : %s",hallwayRelatedFeatures.personId.c_str());
    ROS_INFO("Slope: %f",m);
    ROS_INFO("Intercept: %f",c);
    ROS_INFO("Width: %f",width);
    ROS_INFO("P1 Distance from Left Hallway : %f",p1HallwayL);
    ROS_INFO("P1 Distance from Right Hallway : %f",p1HallwayR);
    ROS_INFO("P1 position (x, y) : %f , %f", peopleData[index].xdistance, peopleData[index].ydistance);
    ROS_INFO("P1 distance from pr2 : %f", peopleData[index].distanceFromPR2);
    outfile << p1ID << ", "<< p1HallwayL << ", " << p1HallwayR << ", " << peopleData[index].distanceFromPR2;*/
    
    for(int innerIndex = 0; innerIndex < noOfPeople; innerIndex++){
      if(innerIndex != index){
        if(flag==0){
          startTime = ros::Time::now().toSec();
          flag = 1;
        }
        ROS_INFO("Value of i : %d", i);
        hallwayRelatedFeatures.frame_id = "base_link";
        p2ID = peopleData[innerIndex].personId.c_str();  
        p2HallwayR = ( abs( m*peopleData[innerIndex].xdistance - peopleData[innerIndex].ydistance + c ) ) / ( sqrt( pow(m,2) + 1 ) );
        p2HallwayL = msg->width_hallway - p2HallwayR;
        
        p1p2Distance = sqrt(pow((peopleData[index].xdistance - peopleData[innerIndex].xdistance),2)+pow((peopleData[index].ydistance - peopleData[innerIndex].ydistance),2));
        currentTime = ros::Time::now().toSec();
        timeStamp = currentTime - startTime;
        
        /*hallwayRelatedFeatures.personId = p2ID;
        hallwayRelatedFeatures.distancefromHallwayR = p2HallwayR;
        hallwayRelatedFeatures.distancefromHallwayL = p2HallwayL;
        hallway_pub.publish(hallwayRelatedFeatures);*/
        
        ROS_INFO("\n\tHallway Features");
        ROS_INFO("Person id : %s",hallwayRelatedFeatures.personId.c_str());
        ROS_INFO("Slope: %f",m);
        ROS_INFO("Intercept: %f",c);
        ROS_INFO("Width: %f",width);
        ROS_INFO("P1 Distance from Left Hallway : %f",p1HallwayL);
        ROS_INFO("P1 Distance from Right Hallway : %f",p1HallwayR);
        ROS_INFO("P2 Distance from Left Hallway : %f",p2HallwayL);
        ROS_INFO("P2 Distance from Right Hallway : %f",p2HallwayR);
        ROS_INFO("P1 position (x, y) : %f , %f", peopleData[index].xdistance, peopleData[index].ydistance);
        ROS_INFO("P2 position (x, y) : %f , %f", peopleData[innerIndex].xdistance, peopleData[innerIndex].ydistance);
        ROS_INFO("P1 P2 interpersonal distance: %f",p1p2Distance);
        ROS_INFO("P1 distance from pr2 : %f", peopleData[index].distanceFromPR2);
        ROS_INFO("P2 distance from pr2 : %f", peopleData[innerIndex].distanceFromPR2);
        ROS_INFO("Time : %f   %f", currentTime, startTime );
        
        time_Stamp = Convert(timeStamp);
        p1_HallwayL= Convert(p1HallwayL);
        p1_HallwayR= Convert(p1HallwayR); 
        p2_HallwayL= Convert(p2HallwayL);
        p2_HallwayR= Convert(p2HallwayR);
        p1p2_Distance= Convert(p1p2Distance);
        p1DistanceFromPR2 = Convert(peopleData[index].distanceFromPR2);
        p2DistanceFromPR2 = Convert(peopleData[innerIndex].distanceFromPR2);  
        
        //To make the file consistent with person
        /*if(p1HallwayL - p2HallwayL >= 60 && peopleFlag == 0){
          if(p1HallwayL < p2HallwayL){
            firstPersonNo = 1;
          }
          else{
            firstPersonNo = 2;
          }
          peopleFlag = 1;
        }
        else */
        if (peopleFlag == 0){
          if(peopleData[index].distanceFromPR2 < peopleData[innerIndex].distanceFromPR2){
            firstPersonNo = 1;
            peopleFlag = 1;
          }
          else{
            firstPersonNo = 2;
            peopleFlag = 1;
          }
        } 
        ROS_INFO("First Person : %d", firstPersonNo );
        
        if(firstPersonNo == 1){
          outfile << timeStamp << ", "<< p1HallwayL << ", " << p1HallwayR << ", " << peopleData[index].distanceFromPR2 << ", "<< p2HallwayL << ", " << p2HallwayR << ", " << peopleData[innerIndex].distanceFromPR2 << ", "<< p1p2Distance  << endl;
          testfile << timeStamp << " , "<< m  << " ,  "<< c << peopleData[index].personId.c_str() << ", "<< " ,  "<< p1HallwayL << " ,  " << p1HallwayR <<  " ,  " << peopleData[index].xdistance << " ,  " << peopleData[index].ydistance << " , " << peopleData[index].distanceFromPR2 << " , "<< peopleData[innerIndex].personId.c_str() << " , "<< p2HallwayL << " , " << p2HallwayR << " , " << peopleData[innerIndex].xdistance << " , " << peopleData[innerIndex].ydistance << " , " << peopleData[innerIndex].distanceFromPR2 <<" , "<< p1p2Distance  << endl;
        }
        else if(firstPersonNo == 2){
          outfile << timeStamp << ", "<< p2HallwayL << ", " << p2HallwayR << ", " << peopleData[innerIndex].distanceFromPR2 << ", "<< p1HallwayL << ", " << p1HallwayR << ", " << peopleData[index].distanceFromPR2 << ", "<< p1p2Distance  << endl;
          testfile << timeStamp << " , "<< m  << " ,  "<< c << peopleData[innerIndex].personId.c_str() << ", "<< " ,  "<< p2HallwayL << " ,  " << p2HallwayR <<  " ,  " << peopleData[innerIndex].xdistance << " ,  " << peopleData[innerIndex].ydistance << " , " << peopleData[innerIndex].distanceFromPR2 << " , "<< peopleData[index].personId.c_str() << " , "<< p1HallwayL << " , " << p1HallwayR << " , " << peopleData[index].xdistance << " , " << peopleData[index].ydistance << " , " << peopleData[index].distanceFromPR2 <<" , "<< p1p2Distance  << endl;
        }
        
        
       //outfile << timeStamp << ", "<< p1HallwayL << ", " << p1HallwayR << ", " << peopleData[index].distanceFromPR2 << ", "<< p2HallwayL << ", " << p2HallwayR << ", " << peopleData[innerIndex].distanceFromPR2 << ", "<< p1p2Distance  << endl;
        //testfile << timeStamp << " , "<< m  << " ,  "<< c << peopleData[index].personId.c_str() << ", "<< " ,  "<< p1HallwayL << " ,  " << p1HallwayR <<  " ,  " << peopleData[index].xdistance << " ,  " << peopleData[index].ydistance << " , " << peopleData[index].distanceFromPR2 << " , "<< peopleData[innerIndex].personId.c_str() << " , "<< p2HallwayL << " , " << p2HallwayR << " , " << peopleData[innerIndex].xdistance << " , " << peopleData[innerIndex].ydistance << " , " << peopleData[innerIndex].distanceFromPR2 <<" , "<< p1p2Distance  << endl;
      }
    }
    
  }
  
}


/*void odomCombinedCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg){
  robotPositionY = msg->pose.pose.position.x * 100;
  robotPositionX = msg->pose.pose.position.y * 100;
}*/

void trajectoryCallback(const geometry_msgs::Point32::ConstPtr& msg){
  float trajectoryX, trajectoryY;
  float personEndPointDistance;
  float personHallwayL, personHallwayR;
  float trajectoryPointHallwayL, trajectoryPointHallwayR;
  float p1EndPointDistance, p2EndPointDistance; 
    
  float timeStamp;
  string p1ID, p2ID;
  int i;
  float p1HallwayL, p1HallwayR, p2HallwayL, p2HallwayR, p1p2Distance; 
  
  trajectoryX = msg->x;
  trajectoryY = msg->y;
  
  for( int index = 0; index < 1; index++){
    p1ID = peopleData[index].personId.c_str();  
    p1HallwayR = ( abs( hallwayData.m*peopleData[index].xdistance - peopleData[index].ydistance + hallwayData.c ) ) / ( sqrt( pow(hallwayData.m,2) + 1 ) );
    p1HallwayL = hallwayData.width - p1HallwayR;
    p1EndPointDistance = sqrt(pow((peopleData[i].posx - trajectoryX),2) + pow((peopleData[i].posy - trajectoryY),2)); //2-D distance
  
    for(int innerIndex = 0; innerIndex < noOfPeople; innerIndex++){
      
      if(innerIndex != index){
        
        if(trajectoryPeopleFlag == 0){
          trajectoryStartTime = ros::Time::now().toSec();
          trajectoryPeopleFlag = 1;
        }
        ROS_INFO("Value of i : %d", i);
        p2ID = peopleData[innerIndex].personId.c_str();  
        p2HallwayR = ( abs( hallwayData.m*peopleData[innerIndex].xdistance - peopleData[innerIndex].ydistance + hallwayData.c ) ) / ( sqrt( pow(hallwayData.m,2) + 1 ) );
        p2HallwayL = hallwayData.width - p2HallwayR;
        p1EndPointDistance = sqrt(pow((peopleData[i].posx - trajectoryX),2) + pow((peopleData[i].posy - trajectoryY),2)); //2-D distance
        
        p1p2Distance = sqrt(pow((peopleData[index].xdistance - peopleData[innerIndex].xdistance),2)+pow((peopleData[index].ydistance - peopleData[innerIndex].ydistance),2));
        trajectoryCurrentTime = ros::Time::now().toSec();
        timeStamp = trajectoryCurrentTime - trajectoryStartTime;
        
        //To make the file consistent with person
        if (trajectoryPeopleFlag == 0){
          if(peopleData[index].distanceFromPR2 < peopleData[innerIndex].distanceFromPR2){
            tracetoryFirstPersonNo = 1;
            peopleFlag = 1;
          }
          else{
            tracetoryFirstPersonNo = 2;
            peopleFlag = 1;
          }
        }
        
        ROS_INFO("First Person : %d", firstPersonNo );
        
        if(firstPersonNo == 1){
          trajectoryOutfile << timeStamp << ", "<< p1HallwayL << ", " << p1HallwayR << ", " << p1EndPointDistance << ", "<< p2HallwayL << ", " << p2HallwayR << ", " << p2EndPointDistance << ", "<< p1p2Distance  << endl;
          trajectoryTestfile << timeStamp << " , "<< hallwayData.m  << " ,  "<< hallwayData.c << peopleData[index].personId.c_str() << ", "<< " ,  "<< p1HallwayL << " ,  " << p1HallwayR <<  " ,  " << peopleData[index].xdistance << " ,  " << peopleData[index].ydistance << " , " << p1EndPointDistance << " , "<< peopleData[innerIndex].personId.c_str() << " , "<< p2HallwayL << " , " << p2HallwayR << " , " << peopleData[innerIndex].xdistance << " , " << peopleData[innerIndex].ydistance << " , " << p2EndPointDistance <<" , "<< p1p2Distance  << endl;
        }
        else if(firstPersonNo == 2){
          trajectoryOutfile << timeStamp << ", "<< p2HallwayL << ", " << p2HallwayR << ", " << p1EndPointDistance << ", "<< p1HallwayL << ", " << p1HallwayR << ", " << p1EndPointDistance << ", "<< p1p2Distance  << endl;
          trajectoryTestfile << timeStamp << " , "<< hallwayData.m  << " ,  "<< hallwayData.c << peopleData[innerIndex].personId.c_str() << ", "<< " ,  "<< p2HallwayL << " ,  " << p2HallwayR <<  " ,  " << peopleData[innerIndex].xdistance << " ,  " << peopleData[innerIndex].ydistance << " , " << p1EndPointDistance << " , "<< peopleData[index].personId.c_str() << " , "<< p1HallwayL << " , " << p1HallwayR << " , " << peopleData[index].xdistance << " , " << peopleData[index].ydistance << " , " << p1EndPointDistance <<" , "<< p1p2Distance  << endl;
        }
        
      }
    }
  }
  
  /*for(int i = 0; i < noOfPeople; i++){
    personEndPointDistance = sqrt(pow((peopleData[i].posx - trajectoryX),2) + pow((peopleData[i].posy - trajectoryY),2)); //2-D distance
    personHallwayR = ( abs(  hallwayData.m*peopleData[i].xdistance - peopleData[i].ydistance +  hallwayData.c ) ) / ( sqrt( pow( hallwayData.m,2) + 1 ) ); //in cm
    personHallwayL = hallwayData.width -  personHallwayR;
    trajectoryPointHallwayR = ( abs(  hallwayData.m*trajectoryX - trajectoryY +  hallwayData.c ) ) / ( sqrt( pow( hallwayData.m,2) + 1 ) ); //in cm
    trajectoryPointHallwayL =  hallwayData.width - trajectoryPointHallwayR;
  }*/
  
  
}



int main( int argc, char* argv[] ){
  
  // Initialize the ROS system and specify the node name.
  ros::init(argc,argv,"legdata") ;
  
  // Establish this program as a ROS node.
  ros::NodeHandle nh ; 
  
  //ros::Subscriber robot_Pose_sub = nh.subscribe("/robot_pose_ekf/odom_combined", 1000, robotPositionCallback);
  ros::Subscriber people_sub = nh.subscribe("people_tracker_measurements", 1000, peoplePositionCallback);
  ros::Subscriber hallway_sub = nh.subscribe("hallway_data", 1000, hallwayDetectionCallback);
  ros::Subscriber trajectory_sub = nh.subscribe("trajectory_points", 1000, trajectoryCallback);
  //ros::Subscriber odom_sub = nh.subscribe("robot_pose_ekf/odom_combined", 1000, odomCombinedCallback);
  
  people_features = nh.advertise<san_feature_extractor::peopleData>("peopleFeatures",1000);
  hallway_pub = nh.advertise<san_feature_extractor::peopleHallwayFeatures>("peopleHallwayFeatures",1000);
  
  ros::spin();
  
  return 0;
}
