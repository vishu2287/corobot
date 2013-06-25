#include "ros/ros.h"

#include <nav_msgs/GetMap.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <Eigen/Geometry>
#include <HectorMapTools.h>
#include <math.h>
#include <tf/transform_datatypes.h>

using namespace std;

double resolution = 0.05; //resolution of the map

/**
 * @brief This node provides images as occupancy grid maps.
 */
class MapAsImageProvider
{
public:
  MapAsImageProvider()
    : pn_("~")
  {

    image_transport_ = new image_transport::ImageTransport(n_);
    image_transport_subscriber_map = image_transport_->subscribe("map_image_raw", 1, &MapAsImageProvider::mapCallback,this); //subscribe to the image topic representing the map
    map_publisher = n_.advertise<nav_msgs::OccupancyGrid>("/map_from_jpeg", 50); // publish the map as an occupancy grid


    ROS_INFO("Image to Map node started.");
  }

  ~MapAsImageProvider()
  {
    delete image_transport_;
  }


  //The map->image conversion runs every time a new map is received
  void mapCallback(const sensor_msgs::ImageConstPtr& image)
  {

    nav_msgs::OccupancyGrid map;
    //fill in some map parameters
    map.header.stamp = image->header.stamp;
    map.header.frame_id = "map";
    map.info.width = image->width;
    map.info.height = image->height;
    map.info.origin.orientation.w = 1;
    map.info.resolution = resolution;
    map.info.origin.position.x = -((image->width + 1) * map.info.resolution * 0.5f);
    map.info.origin.position.y = -((image->height + 1) * map.info.resolution * 0.5f);

    //read the pixels of the image and fill the map table
    int data;
    for(int i=image->height -1; i>=0; i--)
    {
	for (unsigned int j=0; j<image->width;j++)
	{
	    data = image->data[i*image->width + j];
	    if(data >=123 && data <= 131){
	    	map.data.push_back(-1);
	    }
	    else if (data >=251 && data <= 259){
		map.data.push_back(0);
	    }
	    else
		map.data.push_back(100);
	}
    }
    map_publisher.publish(map); //publish the map
  }

  ros::Publisher map_publisher;


  image_transport::Subscriber image_transport_subscriber_map;

  image_transport::ImageTransport* image_transport_;

  ros::NodeHandle n_;
  ros::NodeHandle pn_;

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_to_map_node");

  ros::NodeHandle nh("~");
  nh.param("resolution", resolution, 0.05); 

  MapAsImageProvider map_image_provider;

  ros::spin();

  return 0;
}