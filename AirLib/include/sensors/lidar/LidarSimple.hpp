// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_Lidar_hpp
#define msr_airlib_Lidar_hpp

#include <random>
#include "common/Common.hpp"
#include "LidarSimpleParams.hpp"
#include "LidarBase.hpp"
#include "common/DelayLine.hpp"
#include "common/FrequencyLimiter.hpp"

namespace msr { namespace airlib {

class LidarSimple : public LidarBase {
public:
    LidarSimple(const AirSimSettings::LidarSetting& setting = AirSimSettings::LidarSetting())
        : LidarBase(setting.sensor_name)
    {
        // initialize params
        params_.initializeFromSettings(setting);

        //initialize frequency limiter
        freq_limiter_.initialize(params_.update_frequency, params_.startup_delay, params_.engine_time);
    }

    //*** Start: UpdatableState implementation ***//
    virtual void reset() override
    {
        LidarBase::reset();

        freq_limiter_.reset();
        last_time_ = clock()->nowNanos();
		previous_time_ = clock()->nowNanos();
        updateOutput();
    }

    virtual void update(float delta = 0) override
    {
        LidarBase::update(delta);

        freq_limiter_.update(delta);

        if (freq_limiter_.isWaitComplete()) {
			last_time_ = freq_limiter_.getLastTime();
			previous_time_ = freq_limiter_.getPreviousTime();
            updateOutput();
			if (params_.pause_after_measurement) {
				pause();
			}
        }
    }

    virtual void reportState(StateReporter& reporter) override
    {
        //call base
        LidarBase::reportState(reporter);

        reporter.writeValue("Lidar-NumChannels", params_.number_of_channels);
        reporter.writeValue("Lidar-Range", params_.range);
        reporter.writeValue("Lidar-FOV-Upper", params_.vertical_FOV_upper);
        reporter.writeValue("Lidar-FOV-Lower", params_.vertical_FOV_lower);
    }
    //*** End: UpdatableState implementation ***//

    virtual ~LidarSimple() = default;

    const LidarSimpleParams& getParams() const
    {
        return params_;
    }

protected:
    virtual void getPointCloud(const Pose& lidar_pose, const Pose& vehicle_pose, 
        TTimeDelta delta_time, vector<real_T>& point_cloud) = 0;

	virtual void pause() = 0;

    
private: //methods
    void updateOutput()
    {
        TTimeDelta delta_time = clock()->elapsedBetween(last_time_, previous_time_);

        point_cloud_.clear();

        const GroundTruth& ground_truth = getGroundTruth();

        // calculate the pose before obtaining the point-cloud. Before/after is a bit arbitrary
        // decision here. If the pose can change while obtaining the point-cloud (could happen for drones)
        // then the pose won't be very accurate either way.
        //
        // TODO: Seems like pose is in vehicle inertial-frame (NOT in Global NED frame).
        //    That could be a bit unintuitive but seems consistent with the position/orientation returned as part of 
        //    ImageResponse for cameras and pose returned by getCameraInfo API.
        //    Do we need to convert pose to Global NED frame before returning to clients?
        Pose lidar_pose = params_.relative_pose + ground_truth.kinematics->pose;
        getPointCloud(params_.relative_pose, // relative lidar pose
            ground_truth.kinematics->pose,   // relative vehicle pose
            delta_time, 
            point_cloud_);

        LidarData output;
        output.point_cloud = point_cloud_;
        output.time_stamp = last_time_;
        output.pose = lidar_pose;  

        setOutput(output);
    }

private:
    LidarSimpleParams params_;
    vector<real_T> point_cloud_;

    FrequencyLimiter freq_limiter_;
    TTimePoint last_time_, previous_time_;
};

}} //namespace
#endif
