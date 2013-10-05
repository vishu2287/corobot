#include "corobot_diagnostics/diagnostic.h"

Diagnostic::Diagnostic()
{
    errorIndex = -1;
}

Diagnostic::~Diagnostic()
{
}

void Diagnostic::timerCallback(const ros::TimerEvent&)
// timer callback that takes care of displaying the next message on the lcd
{
    if (errorList.size() > 0)
    {
        if ((++errorIndex) >= errorList.size())
            errorIndex = 0;
        lcd.write(errorList.at(errorIndex));
    }
    else
        lcd.write(std::string("Everything is OK"));
}

void Diagnostic::newError(const std_msgs::String &message)
{
    // Check if we already have that error message in our current list
    bool equal = false;
    for (unsigned int l = 0; l < errorList.size(); l++)
    {
        if (errorList.at(l).compare(message.data) == 0)
        {
	        equal = true;
	    }
	}
	// If we don't, we add it
	if(equal == false)
	{
        	errorList.push_back(message.data);
	}
   
}

void Diagnostic::removeError(const std_msgs::String &message)
{
    // We remove the message from the list, if it exists
    for (unsigned int l = 0; l < errorList.size(); ++l)
    {
        if (errorList.at(l).compare(message.data) == 0)
        {
	        errorList.erase(errorList.begin() + l);
	        break;
	    }
	}
}

int main(int argc, char* argv[])
{
    Diagnostic diagnostic;
	ros::init(argc, argv, "diagnosticLCD");
	ros::NodeHandle n;
	//Create the timer that will display the messages every 5s
	ros::Timer timer = n.createTimer(ros::Duration(1), &Diagnostic::timerCallback, &diagnostic);
	
	// Subscribe to the topics
    	ros::Subscriber newError= n.subscribe("new_diagnostic_error", 1000, &Diagnostic::newError, &diagnostic); 
	ros::Subscriber removeError= n.subscribe("remove_diagnostic_error", 1000, &Diagnostic::removeError, &diagnostic);
	
	ros::spin();
	
	return 0;
	
}
