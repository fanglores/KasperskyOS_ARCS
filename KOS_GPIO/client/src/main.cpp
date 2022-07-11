/*
Implemented by fanglores based on Kaspersky OS Community Edition SDK examples.
Designed for Waveshare Alpha Bot.

Moscow Aviation Institute, 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>

#ifdef __arm__
#include <bsp/bsp.h>
#endif


// simple function for easier console output
void print(const char* msg, const char* prefix = NULL)
{
	if (prefix != NULL) fprintf(stderr, "[%s] ", prefix);

	fprintf(stderr, "%s\n", msg);
}

// GPIO controller class
class GPIOEntity
{
private:
	GpioHandle handle;
	const char* pref = "GPIOEntity";
public:
	GPIOEntity()
	{
		#ifdef __arm__
		{
			if (BspInit(NULL) != BSP_EOK)
			{
				print("Failed to initialize BSP!", pref);
				exit(1);
			}

			if (BspSetConfig("gpio0", "raspberry_pi4b.default") != BSP_EOK)
			{
				print("Failed to set mux configuration for gpio0 module!", pref);
				exit(1);
			}
		}
		#endif

		if (GpioInit())
		{
			print("[CRITICAL ERROR] GpioInit failed!", pref);
			exit(1);
		}

		/* initialize and setup gpio0 port */
		handle = NULL;
		if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE)
		{
			print("GpioOpenPort failed", pref);
			exit(1);
		}
	}

	GpioHandle* getHandle()
	{
		return &handle;
	}

	~GPIOEntity()
	{
		if(GpioClosePort(handle))
	    {
			print("GpioClosePort failed", pref);
	    }
	}
};


// Motor controller class
class Motor
{
private:
	GpioHandle* handle;
	const unsigned int PIN_E, PIN_A, PIN_B;
public:
	Motor(GpioHandle* h, const unsigned int& pin_e, const unsigned int& pin_a, const unsigned int& pin_b) : 
	PIN_E(pin_e), PIN_A(pin_a), PIN_B(pin_b)
	{
		handle = h;
		
		// set pins in the output mode
		GpioSetMode(*handle, PIN_E, GPIO_DIR_OUT);
		GpioSetMode(*handle, PIN_A, GPIO_DIR_OUT);
		GpioSetMode(*handle, PIN_B, GPIO_DIR_OUT);
		
		// enable motors
		GpioOut(*handle, PIN_E, 1);
		GpioOut(*handle, PIN_E, 1);
	}
	
	// doesn`t work currently, we can`t affect PWM
	void setSpeed(const int& speed)
	{
		GpioOut(*handle, PIN_E, speed);
	}
	
	void run(const int& direction)
	{	
		int dir = direction % 2;
		
		GpioOut(*handle, PIN_A, dir);
		GpioOut(*handle, PIN_B, (dir + 1) % 2);
	}
	
	void stop()
	{
		GpioOut(*handle, PIN_A, 0);
		GpioOut(*handle, PIN_B, 0);
	}
};


// Basic robot class
class Robot
{
private:
	// TCP is not implemented in here
	// TCPEntity* cmdrcv;

	// GPIO module on RPi 4B
	GPIOEntity* gpio_entity;

	// two DC motors on the robot
	Motor* motorA;
	Motor* motorB;
	
	// Distation*wheel_value = time of motor work to overcome that distance. So, wheel_value = 1/speed.
	// Probably encoders will be used in the later versions of the implementation.
	const double wheel_value = 0.005;
public:
	Robot(GPIOEntity* ge, Motor* ma, Motor* mb, int spd = 50) : gpio_entity(ge), motorA(ma), motorB(mb)
	{}
	
	void forward(const int& dist)
	{
		motorA->run(0);
		motorB->run(0);
		usleep(wheel_value*dist);
		motorA->stop();
		motorB->stop();
	}
	
	void backward(const int& dist)
	{
		motorA->run(1);
		motorB->run(1);
		usleep(wheel_value* dist);
		motorA->stop();
		motorB->stop();
	}
	
	void turn_left(const int& dist)
	{
		motorA->run(0);
		motorB->run(1);
		usleep(wheel_value* dist);
		motorA->stop();
		motorB->stop();
	}
	
	void turn_right(const int& dist)
	{
		motorA->run(1);
		motorB->run(0);
		usleep(wheel_value* dist);
		motorA->stop();
		motorB->stop();
	}
};


int main(int argc, const char *argv[])
{
	print("\n\nRobo Core Oleg 2022");
	print("Program is starting...\n");
	

	// modules initialisation
	GPIOEntity* ge = new GPIOEntity();
	// TCP init will be here
	Motor* ma = new Motor(ge->getHandle(), 6, 12, 13);
	Motor* mb = new Motor(ge->getHandle(), 26, 20, 21);

	Robot* Oleg = new Robot(ge, ma, mb); 
	
	print("\nGeneral initialisation completed!\n");
	

	// Robot driving tests
	print("Starting GPIO test sequence:", "[TEST]");
	print("\nRobot test: forward");
	Oleg->forward(200);
	
	print("\nRobot test: left");
	Oleg->turn_left(200);
	
	print("\nRobot test: right");
	Oleg->turn_right(200);
	
	print("\nRobot test: backward");
	Oleg->backward(200);   	
    	
	print("\nGpio test completed!", "[TEST]");


	// program shutdown
    print("Program is shutting down!");

	delete Oleg;
	delete ma, mb;
	delete ge;

    return 0;
}
