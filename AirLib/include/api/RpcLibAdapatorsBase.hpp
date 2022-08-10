// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_RpcLibAdapatorsBase_hpp
#define air_RpcLibAdapatorsBase_hpp

#include "common/Common.hpp"
#include "common/CommonStructs.hpp"
#include "physics/Kinematics.hpp"
#include "physics/Environment.hpp"
#include "common/ImageCaptureBase.hpp"
#include "safety/SafetyEval.hpp"
#include "api/WorldSimApiBase.hpp"

#include "common/common_utils/WindowsApisCommonPre.hpp"
#include "rpc/msgpack.hpp"
#include "common/common_utils/WindowsApisCommonPost.hpp"

namespace msr { namespace airlib_rpclib {

class RpcLibAdapatorsBase {
public:
    template<typename TSrc, typename TDest>
    static void to(const std::vector<TSrc>& s, std::vector<TDest>& d)
    {
        d.clear();
        for (size_t i = 0; i < s.size(); ++i)
            d.push_back(s.at(i).to());
    }

    template<typename TSrc, typename TDest>
    static void from(const std::vector<TSrc>& s, std::vector<TDest>& d)
    {
        d.clear();
        for (size_t i = 0; i < s.size(); ++i)
            d.push_back(TDest(s.at(i)));
    }

    struct Vector3r {
        msr::airlib::real_T x_val = 0, y_val = 0, z_val = 0;
        MSGPACK_DEFINE_MAP(x_val, y_val, z_val);

        Vector3r()
        {}

        Vector3r(const msr::airlib::Vector3r& s)
        {
            x_val = s.x();
            y_val = s.y();
            z_val = s.z();
        }
        msr::airlib::Vector3r to() const
        {
            return msr::airlib::Vector3r(x_val, y_val, z_val);
        }
    };

    struct CollisionInfo {
        bool has_collided = false;
        Vector3r normal;
        Vector3r impact_point;
        Vector3r position;
        msr::airlib::real_T penetration_depth = 0;
        msr::airlib::TTimePoint time_stamp = 0;
        std::string object_name;
        int object_id = -1;

        MSGPACK_DEFINE_MAP(has_collided, penetration_depth, time_stamp, normal, impact_point, position, object_name, object_id);
        
        CollisionInfo()
        {}

        CollisionInfo(const msr::airlib::CollisionInfo& s)
        {
            has_collided = s.has_collided;
            normal = s.normal;
            impact_point = s.impact_point;
            position = s.position;
            penetration_depth = s.penetration_depth;
            time_stamp = s.time_stamp;
            object_name = s.object_name;
            object_id = s.object_id;
        }

        msr::airlib::CollisionInfo to() const
        {
            return msr::airlib::CollisionInfo(has_collided, normal.to(), impact_point.to(), position.to(),
                penetration_depth, time_stamp, object_name, object_id);
        }
    };

    struct Quaternionr {
        msr::airlib::real_T w_val = 1, x_val = 0, y_val = 0, z_val = 0;
        MSGPACK_DEFINE_MAP(w_val, x_val, y_val, z_val);

        Quaternionr()
        {}

        Quaternionr(const msr::airlib::Quaternionr& s)
        {
            w_val = s.w();
            x_val = s.x();
            y_val = s.y();
            z_val = s.z();
        }
        msr::airlib::Quaternionr to() const
        {
            return msr::airlib::Quaternionr(w_val, x_val, y_val, z_val);
        }
    };

    struct Pose {
        Vector3r position;
        Quaternionr orientation;
        MSGPACK_DEFINE_MAP(position, orientation);

        Pose()
        {}
        Pose(const msr::airlib::Pose& s)
        {
            position = s.position;
            orientation = s.orientation;
        }
        msr::airlib::Pose to() const
        {
            return msr::airlib::Pose(position.to(), orientation.to());
        }
    };

    struct GeoPoint {
        double latitude = 0, longitude = 0;
        float altitude = 0;
        MSGPACK_DEFINE_MAP(latitude, longitude, altitude);

        GeoPoint()
        {}

        GeoPoint(const msr::airlib::GeoPoint& s)
        {
            latitude = s.latitude;
            longitude = s.longitude;
            altitude = s.altitude;
        }
        msr::airlib::GeoPoint to() const
        {
            return msr::airlib::GeoPoint(latitude, longitude, altitude);
        }
    };

    struct RCData {
        uint64_t timestamp = 0;
        float pitch = 0, roll = 0, throttle = 0, yaw = 0;
        float left_z = 0, right_z = 0;
        uint16_t switches = 0;
        std::string vendor_id = "";
        bool is_initialized = false; //is RC connected?
        bool is_valid = false; //must be true for data to be valid

        MSGPACK_DEFINE_MAP(timestamp, pitch, roll, throttle, yaw, left_z, right_z, switches, vendor_id, is_initialized, is_valid);

        RCData()
        {}

        RCData(const msr::airlib::RCData& s)
        {
            timestamp = s.timestamp;
            pitch = s.pitch;
            roll = s.roll;
            throttle = s.throttle;
            yaw = s.yaw;
            left_z = s.left_z;
            right_z = s.right_z;
            switches = s.switches;
            vendor_id = s.vendor_id;
            is_initialized = s.is_initialized;
            is_valid = s.is_valid;

        }
        msr::airlib::RCData to() const
        {
            msr::airlib::RCData d;
            d.timestamp = timestamp;
            d.pitch = pitch;
            d.roll = roll;
            d.throttle = throttle;
            d.yaw = yaw;
            d.left_z = left_z;
            d.right_z = right_z;
            d.switches = switches;
            d.vendor_id = vendor_id;
            d.is_initialized = is_initialized;
            d.is_valid = is_valid;
            
            return d;
        }
    };

    struct ProjectionMatrix {
        float matrix[4][4];

        MSGPACK_DEFINE_MAP(matrix);

        ProjectionMatrix()
        {
        }

        ProjectionMatrix(const msr::airlib::ProjectionMatrix& s)
        {
            for (auto i = 0; i < 4; ++i)
                for (auto j = 0; j < 4; ++j)
                    matrix[i][j] = s.matrix[i][j];
        }

        msr::airlib::ProjectionMatrix to() const
        {
            msr::airlib::ProjectionMatrix s;
            for (auto i = 0; i < 4; ++i)
                for (auto j = 0; j < 4; ++j)
                    s.matrix[i][j] = matrix[i][j];
            return s;
        }
    };

    struct CameraInfo {
        Pose pose;
        float fov;
        ProjectionMatrix proj_mat;

        MSGPACK_DEFINE_MAP(pose, fov, proj_mat);

        CameraInfo()
        {}

        CameraInfo(const msr::airlib::CameraInfo& s)
        {
            pose = s.pose;
            fov = s.fov;
            proj_mat = ProjectionMatrix(s.proj_mat);
        }

        msr::airlib::CameraInfo to() const
        {
            msr::airlib::CameraInfo s;
            s.pose = pose.to();
            s.fov = fov;
            s.proj_mat = proj_mat.to();

            return s;
        }
    };
    
    struct KinematicsState {
        Vector3r position;
        Quaternionr orientation;

        Vector3r linear_velocity;
        Vector3r angular_velocity;

        Vector3r linear_acceleration;
        Vector3r angular_acceleration;

        MSGPACK_DEFINE_MAP(position, orientation, linear_velocity, angular_velocity, linear_acceleration, angular_acceleration);


        KinematicsState()
        {}

        KinematicsState(const msr::airlib::Kinematics::State& s)
        {
            position = s.pose.position;
            orientation = s.pose.orientation;
            linear_velocity = s.twist.linear;
            angular_velocity = s.twist.angular;
            linear_acceleration = s.accelerations.linear;
            angular_acceleration = s.accelerations.angular;
        }

        msr::airlib::Kinematics::State to() const
        {
            msr::airlib::Kinematics::State s;
            s.pose.position = position.to();
            s.pose.orientation = orientation.to();
            s.twist.linear = linear_velocity.to();
            s.twist.angular = angular_velocity.to();
            s.accelerations.linear = linear_acceleration.to();
            s.accelerations.angular = angular_acceleration.to();

            return s;
        }
    };

    struct EnvironmentState {
        Vector3r position;
        GeoPoint geo_point;

        //these fields are computed
        Vector3r gravity;
        float air_pressure;
        float temperature;
        float air_density;

        MSGPACK_DEFINE_MAP(position, geo_point, gravity, air_pressure, temperature, air_density);

        EnvironmentState()
        {}

        EnvironmentState(const msr::airlib::Environment::State& s)
        {
            position = s.position;
            geo_point = s.geo_point;
            gravity = s.gravity;
            air_pressure = s.air_pressure;
            temperature = s.temperature;
            air_density = s.air_density;
        }

        msr::airlib::Environment::State to() const
        {
            msr::airlib::Environment::State s;
            s.position = position.to();
            s.geo_point = geo_point.to();
            s.gravity = gravity.to();
            s.air_pressure = air_pressure;
            s.temperature = temperature;
            s.air_density = air_density;

            return s;
        }
    };

    struct ImageRequest {
        std::string camera_name;
        msr::airlib::ImageCaptureBase::ImageType image_type;
        bool pixels_as_float;
        bool compress;

        MSGPACK_DEFINE_MAP(camera_name, image_type, pixels_as_float, compress);

        ImageRequest()
        {}

        ImageRequest(const msr::airlib::ImageCaptureBase::ImageRequest& s)
        {
            camera_name = s.camera_name;
            image_type = s.image_type;
            pixels_as_float = s.pixels_as_float;
            compress = s.compress;
        }

        msr::airlib::ImageCaptureBase::ImageRequest to() const
        {
            msr::airlib::ImageCaptureBase::ImageRequest d;
            d.camera_name = camera_name;
            d.image_type = image_type;
            d.pixels_as_float = pixels_as_float;
            d.compress = compress;

            return d;
        }

        static std::vector<ImageRequest> from(
            const std::vector<msr::airlib::ImageCaptureBase::ImageRequest>& request
        ) {
            std::vector<ImageRequest> request_adaptor;
            for (const auto& item : request)
                request_adaptor.push_back(ImageRequest(item));

            return request_adaptor;
        }
        static std::vector<msr::airlib::ImageCaptureBase::ImageRequest> to(
            const std::vector<ImageRequest>& request_adapter
        ) {
            std::vector<msr::airlib::ImageCaptureBase::ImageRequest> request;
            for (const auto& item : request_adapter)
                request.push_back(item.to());

            return request;
        }         
    };

    struct ImageResponse {
        std::vector<uint8_t> image_data_uint8;
        std::vector<float> image_data_float;

        std::string camera_name;
        Vector3r camera_position;
        Quaternionr camera_orientation;
        msr::airlib::TTimePoint time_stamp;
        std::string message;
        bool pixels_as_float;
        bool compress;
        int width, height;
        msr::airlib::ImageCaptureBase::ImageType image_type;

        MSGPACK_DEFINE_MAP(image_data_uint8, image_data_float, camera_position, camera_name,
            camera_orientation, time_stamp, message, pixels_as_float, compress, width, height, image_type);

        ImageResponse()
        {}

        ImageResponse(const msr::airlib::ImageCaptureBase::ImageResponse& s)
        {
            pixels_as_float = s.pixels_as_float;
            
            image_data_uint8 = s.image_data_uint8;
            image_data_float = s.image_data_float;

            //TODO: remove bug workaround for https://github.com/rpclib/rpclib/issues/152
            if (image_data_uint8.size() == 0)
                image_data_uint8.push_back(0);
            if (image_data_float.size() == 0)
                image_data_float.push_back(0);

            camera_name = s.camera_name;
            camera_position = Vector3r(s.camera_position);
            camera_orientation = Quaternionr(s.camera_orientation);
            time_stamp = s.time_stamp;
            message = s.message;
            compress = s.compress;
            width = s.width;
            height = s.height;
            image_type = s.image_type;
        }

        msr::airlib::ImageCaptureBase::ImageResponse to() const
        {
            msr::airlib::ImageCaptureBase::ImageResponse d;

            d.pixels_as_float = pixels_as_float;

            if (! pixels_as_float)
                d.image_data_uint8 = image_data_uint8;
            else
                d.image_data_float = image_data_float;

            d.camera_name = camera_name;
            d.camera_position = camera_position.to();
            d.camera_orientation = camera_orientation.to();
            d.time_stamp = time_stamp;
            d.message = message;
            d.compress = compress;
            d.width = width;
            d.height = height;
            d.image_type = image_type;

            return d;
        }

        static std::vector<msr::airlib::ImageCaptureBase::ImageResponse> to(
            const std::vector<ImageResponse>& response_adapter
        ) {
            std::vector<msr::airlib::ImageCaptureBase::ImageResponse> response;
            for (const auto& item : response_adapter)
                response.push_back(item.to());

            return response;
        }
        static std::vector<ImageResponse> from(
            const std::vector<msr::airlib::ImageCaptureBase::ImageResponse>& response
        ) {
            std::vector<ImageResponse> response_adapter;
            for (const auto& item : response)
                response_adapter.push_back(ImageResponse(item));

            return response_adapter;
        }
    };

    struct LidarData {

        msr::airlib::TTimePoint time_stamp;    // timestamp
        std::vector<float> point_cloud;        // data
		std::vector<std::string> groundtruth;  // ground truth labels

        Pose pose;

        MSGPACK_DEFINE_MAP(time_stamp, point_cloud, groundtruth, pose);

        LidarData()
        {}

        LidarData(const msr::airlib::LidarData& s)
        {
            time_stamp = s.time_stamp;
            point_cloud = s.point_cloud;
			groundtruth = s.groundtruth;

            //TODO: remove bug workaround for https://github.com/rpclib/rpclib/issues/152
            if (point_cloud.size() == 0)
                point_cloud.push_back(0);
            pose = s.pose;
        }

        msr::airlib::LidarData to() const
        {
            msr::airlib::LidarData d;

            d.time_stamp = time_stamp;
            d.point_cloud = point_cloud;
			d.groundtruth = groundtruth;
            d.pose = pose.to();

            return d;
        }
    };

	struct GPULidarData {

		msr::airlib::TTimePoint time_stamp;    // timestamp
		std::vector<float> point_cloud;        // data

		Pose pose;

		MSGPACK_DEFINE_MAP(time_stamp, point_cloud, pose);

		GPULidarData()
		{}

		GPULidarData(const msr::airlib::GPULidarData& s)
		{
			time_stamp = s.time_stamp;
			point_cloud = s.point_cloud;

			//TODO: remove bug workaround for https://github.com/rpclib/rpclib/issues/152
			if (point_cloud.size() == 0)
				point_cloud.push_back(0);
			pose = s.pose;
		}

		msr::airlib::GPULidarData to() const
		{
			msr::airlib::GPULidarData d;

			d.time_stamp = time_stamp;
			d.point_cloud = point_cloud;
			d.pose = pose.to();

			return d;
		}
	};

	struct EchoData {

		msr::airlib::TTimePoint time_stamp;    // timestamp
		std::vector<float> point_cloud;        // data
		Pose pose;

		MSGPACK_DEFINE_MAP(time_stamp, point_cloud, pose);

		EchoData()
		{}

		EchoData(const msr::airlib::EchoData& s)
		{
			time_stamp = s.time_stamp;
			point_cloud = s.point_cloud;

			//TODO: remove bug workaround for https://github.com/rpclib/rpclib/issues/152
			if (point_cloud.size() == 0)
				point_cloud.push_back(0);

			pose = s.pose;
		}

		msr::airlib::EchoData to() const
		{
			msr::airlib::EchoData d;

			d.time_stamp = time_stamp;
			d.point_cloud = point_cloud;
			d.pose = pose.to();

			return d;
		}
	};

    struct SensorTemplateData {

        msr::airlib::TTimePoint time_stamp;    // timestamp
        std::vector<float> point_cloud;        // data
        Pose pose;

        MSGPACK_DEFINE_MAP(time_stamp, point_cloud, pose);

        SensorTemplateData()
        {}

        SensorTemplateData(const msr::airlib::SensorTemplateData& s)
        {
            time_stamp = s.time_stamp;
            point_cloud = s.point_cloud;

            //TODO: remove bug workaround for https://github.com/rpclib/rpclib/issues/152
            if (point_cloud.size() == 0)
                point_cloud.push_back(0);

            pose = s.pose;
        }

        msr::airlib::SensorTemplateData to() const
        {
            msr::airlib::SensorTemplateData d;

            d.time_stamp = time_stamp;
            d.point_cloud = point_cloud;
            d.pose = pose.to();

            return d;
        }
    };

    struct MarLocUwbSensorData {

        msr::airlib::TTimePoint time_stamp;    // timestamp
        //std::vector<float> point_cloud;        // data
        Pose pose;
        std::vector<float> allBeaconsId, allBeaconsX, allBeaconsY, allBeaconsZ;
        
        MSGPACK_DEFINE(time_stamp, pose, allBeaconsId, allBeaconsX, allBeaconsY, allBeaconsZ);

        MarLocUwbSensorData()
        {}

        MarLocUwbSensorData(const msr::airlib::MarLocUwbSensorData& s)
        {
            time_stamp = s.time_stamp;
            pose = s.pose;
            allBeaconsId = s.allBeaconsId;
            allBeaconsX = s.allBeaconsX;
            allBeaconsY = s.allBeaconsY;
            allBeaconsZ = s.allBeaconsZ;
            
        }

        msr::airlib::MarLocUwbSensorData to() const
        {
            msr::airlib::MarLocUwbSensorData d;

            d.time_stamp = time_stamp;
            d.pose = pose.to();
            d.allBeaconsId = allBeaconsId;
            d.allBeaconsX = allBeaconsX;
            d.allBeaconsY = allBeaconsY;
            d.allBeaconsZ = allBeaconsZ;
            
            return d;
        }
    };
	
    /*struct MarLocUwbRange {
        uint64_t time_stamp;
        int anchorId;
        float anchorX, anchorY, anchorZ;
        bool valid_range;
        float distance;
        float rssi;

        //MSGPACK_DEFINE(time_stamp, anchorId, anchorX, anchorY, anchorZ, valid_range, distance, rssi);

        MarLocUwbRange()
        {}

        MarLocUwbRange(const msr::airlib::MarLocUwbRange& s)
        {
            time_stamp = s.time_stamp;
            anchorId = s.anchorId;
            anchorX = s.anchorX;
            anchorY = s.anchorY;
            anchorZ = s.anchorZ;
            valid_range = s.valid_range;
            distance = s.distance;
            rssi = s.rssi;
        }

        msr::airlib::MarLocUwbRange to() const
        {
            msr::airlib::MarLocUwbRange d;

            d.time_stamp = time_stamp;
            d.anchorId = anchorId;
            d.anchorX = anchorX;
            d.anchorY = anchorY;
            d.anchorZ = anchorZ;
            d.valid_range = valid_range;
            d.distance = distance;
            d.rssi = rssi;

            return d;
        }
    };*/

    /*struct MarLocUwbRangeArray {

        int tagId;
        float tagX, tagY, tagZ;
        std::vector<int> ranges;

        //MSGPACK_DEFINE(tagId, tagX, tagY, tagZ, ranges);

        MarLocUwbRangeArray()
        {}

        MarLocUwbRangeArray(const msr::airlib::MarLocUwbRangeArray& s)
        {
            tagId = s.tagId;
            tagX = s.tagX;
            tagY = s.tagY;
            tagZ = s.tagZ;
            ranges = s.ranges;
        }

        msr::airlib::MarLocUwbRangeArray to() const
        {
            msr::airlib::MarLocUwbRangeArray d;

            d.tagId = tagId;
            d.tagX = tagX;
            d.tagY = tagY;
            d.tagZ = tagZ;
            d.ranges = ranges;

            return d;
        }
    };*/

    /*struct MarLocUwbReturnMessage {
        std::vector<msr::airlib::MarLocUwbRange> marLocUwbRange;
        std::vector<msr::airlib::MarLocUwbRangeArray> marLocUwbRangeArray;
        //int marLocUwbRange;
        //int marLocUwbRangeArray;

        //MSGPACK_DEFINE(marLocUwbRange, marLocUwbRangeArray);

        MarLocUwbReturnMessage()
        {}

        MarLocUwbReturnMessage(const msr::airlib::MarLocUwbReturnMessage& s)
        {
            marLocUwbRange = s.marLocUwbRange;
            marLocUwbRangeArray = s.marLocUwbRangeArray;
        }

        msr::airlib::MarLocUwbReturnMessage to() const
        {
            msr::airlib::MarLocUwbReturnMessage d;

            d.marLocUwbRange = marLocUwbRange;
            d.marLocUwbRangeArray = marLocUwbRangeArray;

            return d;
        }
    };*/
    
    struct MarLocUwbReturnMessage {
        //MarLocUwbRange
        std::vector <uint64_t> mur_time_stamp;
        std::vector<std::string> mur_anchorId;
        std::vector<float> mur_anchorPosX;
        std::vector<float> mur_anchorPosY;
        std::vector<float> mur_anchorPosZ;
        std::vector<bool> mur_valid_range;
        std::vector<float> mur_distance;
        std::vector<float> mur_rssi;

        //MarLocUwbRangeArray
        std::vector<std::string> mura_tagId;
        std::vector<float> mura_tagPosX;
        std::vector<float> mura_tagPosY;
        std::vector<float> mura_tagPosZ;
        std::vector <std::vector<int>> mura_ranges;

        //std::vector<Pose> pose;

        MSGPACK_DEFINE_MAP(mur_time_stamp, mur_anchorId, mur_anchorPosX, mur_anchorPosY, mur_anchorPosZ, mur_valid_range, mur_distance, mur_rssi, mura_tagId, mura_tagPosX, mura_tagPosY, mura_tagPosZ, mura_ranges);

        MarLocUwbReturnMessage()
        {}

        MarLocUwbReturnMessage(const msr::airlib::MarLocUwbReturnMessage& s)
        {
            mur_time_stamp = s.mur_time_stamp;
            mur_anchorId = s.mur_anchorId;
            mur_anchorPosX = s.mur_anchorPosX;
            mur_anchorPosY = s.mur_anchorPosY;
            mur_anchorPosZ = s.mur_anchorPosZ;
            mur_valid_range = s.mur_valid_range;
            mur_distance = s.mur_distance;
            mur_rssi = s.mur_rssi;
            mura_tagId = s.mura_tagId;
            mura_tagPosX = s.mura_tagPosX;
            mura_tagPosY = s.mura_tagPosY;
            mura_tagPosZ = s.mura_tagPosZ;
            mura_ranges = s.mura_ranges;
            //marLocUwbRange = s.marLocUwbRange;
            //marLocUwbRangeArray = s.marLocUwbRangeArray;

            //pose = s.pose;
        }

        msr::airlib::MarLocUwbReturnMessage to() const
        {
            msr::airlib::MarLocUwbReturnMessage d;

            d.mur_time_stamp = mur_time_stamp;
            d.mur_anchorId = mur_anchorId;
            d.mur_anchorPosX = mur_anchorPosX;
            d.mur_anchorPosY = mur_anchorPosY;
            d.mur_anchorPosZ = mur_anchorPosZ;
            d.mur_valid_range = mur_valid_range;
            d.mur_distance = mur_distance;
            d.mur_rssi = mur_rssi;
            d.mura_tagId = mura_tagId;
            d.mura_tagPosX = mura_tagPosX;
            d.mura_tagPosY = mura_tagPosY;
            d.mura_tagPosZ = mura_tagPosZ;
            d.mura_ranges = mura_ranges;
            //d.marLocUwbRange = marLocUwbRange;
            //d.marLocUwbRangeArray = marLocUwbRangeArray;
            //d.pose = pose;
            return d;
        }
    };

    struct WifiSensorData {

        msr::airlib::TTimePoint time_stamp;    // timestamp
        //std::vector<float> point_cloud;        // data
        Pose pose;
        std::vector<float> allBeaconsId, allBeaconsX, allBeaconsY, allBeaconsZ;

        MSGPACK_DEFINE(time_stamp, pose, allBeaconsId, allBeaconsX, allBeaconsY, allBeaconsZ);

        WifiSensorData()
        {}

        WifiSensorData(const msr::airlib::WifiSensorData& s)
        {
            time_stamp = s.time_stamp;
            pose = s.pose;
            allBeaconsId = s.allBeaconsId;
            allBeaconsX = s.allBeaconsX;
            allBeaconsY = s.allBeaconsY;
            allBeaconsZ = s.allBeaconsZ;
        }

        msr::airlib::WifiSensorData to() const
        {
           msr::airlib::WifiSensorData d;

            d.time_stamp = time_stamp;
            d.pose = pose.to();
            d.allBeaconsId = allBeaconsId;
            d.allBeaconsX = allBeaconsX;
            d.allBeaconsY = allBeaconsY;
            d.allBeaconsZ = allBeaconsZ;
            return d;
        }
    };

    struct WifiReturnMessage2 {
        //MarLocUwbRange
        std::vector <uint64_t> wr_time_stamp;
        std::vector<int> wr_anchorId;
        std::vector<float> wr_anchorX, wr_anchorY, wr_anchorZ;
        std::vector<bool> wr_valid_range;
        std::vector<float> wr_distance;
        std::vector<float> wr_rssi;

        //MarLocUwbRangeArray
        std::vector<std::string> wra_tagId;
        std::vector<float> wra_tagX, wra_tagY, wra_tagZ;
        std::vector <std::vector<int>> wra_ranges;

        //std::vector<Pose> pose;

        MSGPACK_DEFINE_MAP(wr_time_stamp, wr_anchorId, wr_anchorX, wr_anchorY, wr_anchorZ, wr_valid_range, wr_distance, wr_rssi, wra_tagId, wra_tagX, wra_tagY, wra_tagZ, wra_ranges);

        WifiReturnMessage2()
        {}

        WifiReturnMessage2(const msr::airlib::WifiReturnMessage2& s)
        {
            wr_time_stamp = s.wr_time_stamp;
            wr_anchorId = s.wr_anchorId;
            wr_anchorX = s.wr_anchorX;
            wr_anchorY = s.wr_anchorY;
            wr_anchorZ = s.wr_anchorZ;
            wr_valid_range = s.wr_valid_range;
            wr_distance = s.wr_distance;
            wr_rssi = s.wr_rssi;
            wra_tagId = s.wra_tagId;
            wra_tagX = s.wra_tagX;
            wra_tagY = s.wra_tagY;
            wra_tagZ = s.wra_tagZ;
            wra_ranges = s.wra_ranges;
            //marLocUwbRange = s.marLocUwbRange;
            //marLocUwbRangeArray = s.marLocUwbRangeArray;

            //pose = s.pose;
        }

        msr::airlib::WifiReturnMessage2 to() const
        {
            msr::airlib::WifiReturnMessage2 d;

            d.wr_time_stamp = wr_time_stamp;
            d.wr_anchorId = wr_anchorId;
            d.wr_anchorX = wr_anchorX;
            d.wr_anchorY = wr_anchorY;
            d.wr_anchorZ = wr_anchorZ;
            d.wr_valid_range = wr_valid_range;
            d.wr_distance = wr_distance;
            d.wr_rssi = wr_rssi;
            d.wra_tagId = wra_tagId;
            d.wra_tagX = wra_tagX;
            d.wra_tagY = wra_tagY;
            d.wra_tagZ = wra_tagZ;
            d.wra_ranges = wra_ranges;
            return d;
        }
    };

    struct ImuData {
        msr::airlib::TTimePoint time_stamp;
        Quaternionr orientation;
        Vector3r angular_velocity;
        Vector3r linear_acceleration;

        MSGPACK_DEFINE_MAP(time_stamp, orientation, angular_velocity, linear_acceleration);

        ImuData()
        {}

        ImuData(const msr::airlib::ImuBase::Output& s)
        {
            time_stamp = s.time_stamp;
            orientation = s.orientation;
            angular_velocity = s.angular_velocity;
            linear_acceleration = s.linear_acceleration;
        }

        msr::airlib::ImuBase::Output to() const
        {
            msr::airlib::ImuBase::Output d;

            d.time_stamp = time_stamp;
            d.orientation = orientation.to();
            d.angular_velocity = angular_velocity.to();
            d.linear_acceleration = linear_acceleration.to();

            return d;
        }
    };

    struct BarometerData {
        msr::airlib::TTimePoint time_stamp;
        msr::airlib::real_T altitude;
        msr::airlib::real_T pressure;
        msr::airlib::real_T qnh;

        MSGPACK_DEFINE_MAP(time_stamp, altitude, pressure, qnh);

        BarometerData()
        {}

        BarometerData(const msr::airlib::BarometerBase::Output& s)
        {
            time_stamp = s.time_stamp;
            altitude = s.altitude;
            pressure = s.pressure;
            qnh = s.qnh;
        }

        msr::airlib::BarometerBase::Output to() const
        {
            msr::airlib::BarometerBase::Output d;

            d.time_stamp = time_stamp;
            d.altitude = altitude;
            d.pressure = pressure;
            d.qnh = qnh;

            return d;
        }
    };

    struct MagnetometerData {
        msr::airlib::TTimePoint time_stamp;
        Vector3r magnetic_field_body;
        std::vector<float> magnetic_field_covariance; // not implemented in MagnetometerBase.hpp

        MSGPACK_DEFINE_MAP(time_stamp, magnetic_field_body, magnetic_field_covariance);

        MagnetometerData()
        {}

        MagnetometerData(const msr::airlib::MagnetometerBase::Output& s)
        {
            time_stamp = s.time_stamp;
            magnetic_field_body = s.magnetic_field_body;
            magnetic_field_covariance = s.magnetic_field_covariance;
        }

        msr::airlib::MagnetometerBase::Output to() const
        {
            msr::airlib::MagnetometerBase::Output d;

            d.time_stamp = time_stamp;
            d.magnetic_field_body = magnetic_field_body.to();
            d.magnetic_field_covariance = magnetic_field_covariance;

            return d;
        }
    };

    struct GnssReport {
        GeoPoint geo_point;
        msr::airlib::real_T eph = 0.0, epv = 0.0; 
        Vector3r velocity;
        msr::airlib::GpsBase::GnssFixType fix_type;
        uint64_t time_utc = 0;

        MSGPACK_DEFINE_MAP(geo_point, eph, epv, velocity, fix_type, time_utc);

        GnssReport()
        {}

        GnssReport(const msr::airlib::GpsBase::GnssReport& s)
        {
            geo_point = s.geo_point;
            eph = s.eph;
            epv = s.epv;
            velocity = s.velocity;
            fix_type = s.fix_type;
            time_utc = s.time_utc;
        }

        msr::airlib::GpsBase::GnssReport to() const
        {
            msr::airlib::GpsBase::GnssReport d;

            d.geo_point = geo_point.to();
            d.eph = eph;
            d.epv = epv;
            d.velocity = velocity.to();
            d.fix_type = fix_type;
            d.time_utc = time_utc;

            return d;
        }
    };

    struct GpsData {
        msr::airlib::TTimePoint time_stamp;
        GnssReport gnss;
        bool is_valid = false;

        MSGPACK_DEFINE_MAP(time_stamp, gnss, is_valid);

        GpsData()
        {}

        GpsData(const msr::airlib::GpsBase::Output& s)
        {
            time_stamp = s.time_stamp;
            gnss = s.gnss;
            is_valid = s.is_valid;
        }

        msr::airlib::GpsBase::Output to() const
        {
            msr::airlib::GpsBase::Output d;

            d.time_stamp = time_stamp;
            d.gnss = gnss.to();
            d.is_valid = is_valid;

            return d;
        }
    };

    struct DistanceSensorData {
        msr::airlib::TTimePoint time_stamp;
        msr::airlib::real_T distance;    //meters
        msr::airlib::real_T min_distance;//m
        msr::airlib::real_T max_distance;//m
        Pose relative_pose;

        MSGPACK_DEFINE_MAP(time_stamp, distance, min_distance, max_distance, relative_pose);

        DistanceSensorData()
        {}

        DistanceSensorData(const msr::airlib::DistanceBase::Output& s)
        {
            time_stamp = s.time_stamp;
            distance = s.distance;
            min_distance = s.min_distance;
            max_distance = s.max_distance;
            relative_pose = s.relative_pose;
        }

        msr::airlib::DistanceBase::Output to() const
        {
            msr::airlib::DistanceBase::Output d;

            d.time_stamp = time_stamp;
            d.distance = distance;
            d.min_distance = min_distance;
            d.max_distance = max_distance;
            d.relative_pose = relative_pose.to();

            return d;
        }
    };
};

}} //namespace

MSGPACK_ADD_ENUM(msr::airlib::SafetyEval::SafetyViolationType_);
MSGPACK_ADD_ENUM(msr::airlib::SafetyEval::ObsAvoidanceStrategy);
MSGPACK_ADD_ENUM(msr::airlib::ImageCaptureBase::ImageType);
MSGPACK_ADD_ENUM(msr::airlib::WorldSimApiBase::WeatherParameter);
MSGPACK_ADD_ENUM(msr::airlib::GpsBase::GnssFixType);

#endif
