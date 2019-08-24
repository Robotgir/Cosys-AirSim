// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_EchoSimpleParams_hpp
#define msr_airlib_EchoSimpleParams_hpp

#include "common/Common.hpp"
#include "common/AirSimSettings.hpp"

namespace msr { namespace airlib {

struct EchoSimpleParams {

    // default settings
    // TODO: enable reading of these params from AirSim settings

    uint number_of_traces = 1000;           // Amount of traces (rays) being cast 
	real_T attenuation_per_distance;		// Attenuation of signal wrt distance traveled (dB/m)
	real_T attenuation_per_reflection;		// Attenuation of signal wrt reflections (dB)
	real_T attenuation_limit;				// Attenuation at which the signal is considered dissipated (dB)
	real_T measurement_frequency = 10;		// The frequency of the sensor (measurements/s)
	real_T sensor_diameter = 0.5;			// The diameter of the sensor plane used to capture the reflecting traces (meter)
	bool pause_after_measurement = false;	// Pause the simulation after each measurement. Useful for API interaction to be synced
	bool engine_time = true;				// If false, real-time simulation will be used for timestamps and measurement frequency
										    // If true, the time passed in-engine will be used (when performance doesn't allow real-time operation)

	std::string name = "EchoSensor";

    Pose relative_pose {
        Vector3r(0,0,-1),                   // position - a little above vehicle (especially for cars) or Vector3r::Zero()
        Quaternionr::Identity()             // orientation - by default Quaternionr(1, 0, 0, 0) 
        };                       

	bool draw_reflected_points = true;		// Draw debug points in world where reflected points are captured by the echo sensor
	bool draw_reflected_lines = false;		// Draw debug lines in world from reflected points to the echo sensor
	bool draw_initial_points = false;		// Draw the points of the initial half sphere where the traces (rays) are cast
	bool draw_bounce_lines = false;			// Draw lines of all bouncing reflections of the traces with their color depending on attenuation
	bool draw_sensor = false;				// Draw the physical sensor in the world on the vehicle


    std::string data_frame = AirSimSettings::kVehicleInertialFrame;

    real_T update_frequency = measurement_frequency;       // polling rate of update function, in Hz
    real_T startup_delay = 0;                              // startup delay of sensor, in sec

    void initializeFromSettings(const AirSimSettings::EchoSetting& settings)
    {
        std::string simmode_name = AirSimSettings::singleton().simmode_name;

		number_of_traces = settings.number_of_traces;
		attenuation_per_distance = settings.attenuation_per_distance;
		attenuation_per_reflection = settings.attenuation_per_reflection;
		attenuation_limit = settings.attenuation_limit;
		measurement_frequency = settings.measurement_frequency;
		sensor_diameter = settings.sensor_diameter;
		pause_after_measurement = settings.pause_after_measurement;

		name = settings.sensor_name;

        relative_pose.position = settings.position;
        if (std::isnan(relative_pose.position.x()))
            relative_pose.position.x() = 0;
        if (std::isnan(relative_pose.position.y()))
            relative_pose.position.y() = 0;
        if (std::isnan(relative_pose.position.z())) {
            if (simmode_name == "Multirotor")
                relative_pose.position.z() = 0;
            else
                relative_pose.position.z() = -1;  // a little bit above for cars
        }

        float pitch, roll, yaw;
        pitch = !std::isnan(settings.rotation.pitch) ? settings.rotation.pitch : 0;
        roll = !std::isnan(settings.rotation.roll) ? settings.rotation.roll : 0;
        yaw = !std::isnan(settings.rotation.yaw) ? settings.rotation.yaw : 0;
        relative_pose.orientation = VectorMath::toQuaternion(
            Utils::degreesToRadians(pitch),   //pitch - rotation around Y axis
            Utils::degreesToRadians(roll),    //roll  - rotation around X axis
            Utils::degreesToRadians(yaw)	  //yaw   - rotation around Z axis
		);   
           
        draw_reflected_points = settings.draw_reflected_points;
		draw_reflected_lines = settings.draw_reflected_lines;
		draw_initial_points = settings.draw_initial_points;
		draw_bounce_lines = settings.draw_bounce_lines;
		draw_sensor = settings.draw_sensor;

		engine_time = settings.engine_time;
        data_frame = settings.data_frame;
		update_frequency = settings.measurement_frequency;
    }
};

}} //namespace
#endif
