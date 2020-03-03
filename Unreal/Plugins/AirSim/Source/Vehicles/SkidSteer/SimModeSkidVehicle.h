// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimMode/SimModeBase.h"
#include "SkidVehiclePawn.h"
#include "common/Common.hpp"
#include "SkidVehiclePawnSimApi.h"
#include "api/VehicleSimApiBase.hpp"
#include "Vehicles/AirSimVehicle.h"
#include "Runtime/Engine/Public/EngineUtils.h"

#include "SimModeSkidVehicle.generated.h"

/**
 * 
 */
UCLASS()
class AIRSIM_API ASimModeSkidVehicle : public ASimModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual bool isPaused() const override;
	virtual void pause(bool is_paused) override;
	virtual void continueForTime(double seconds) override;

private:
	typedef msr::airlib::ClockFactory ClockFactory;
	typedef common_utils::Utils Utils;
	typedef msr::airlib::TTimePoint TTimePoint;
	typedef msr::airlib::TTimeDelta TTimeDelta;
	typedef ASkidVehiclePawn TVehiclePawn;
	typedef msr::airlib::VehicleSimApiBase VehicleSimApiBase;
	typedef msr::airlib::VectorMath VectorMath;
	typedef msr::airlib::Vector3r Vector3r;

private:
	void initializePauseState();

protected:
	virtual void setupClockSpeed() override;
	virtual std::unique_ptr<msr::airlib::ApiServerBase> createApiServer() const override;
	virtual void getExistingVehiclePawns(TArray<AirsimVehicle*>& pawns) const override;
	virtual bool isVehicleTypeSupported(const std::string& vehicle_type) const override;
	virtual std::string getVehiclePawnPathName(const AirSimSettings::VehicleSetting& vehicle_setting) const override;
	virtual PawnEvents* getVehiclePawnEvents(APawn* pawn) const override;
	virtual const common_utils::UniqueValueMap<std::string, APIPCamera*> getVehiclePawnCameras(APawn* pawn) const override;
	virtual void initializeVehiclePawn(APawn* pawn) override;
	virtual std::unique_ptr<PawnSimApi> createVehicleSimApi(
		const PawnSimApi::Params& pawn_sim_api_params) const override;
	virtual msr::airlib::VehicleApiBase* getVehicleApi(const PawnSimApi::Params& pawn_sim_api_params,
		const PawnSimApi* sim_api) const override;

private:
	std::atomic<float> current_clockspeed_;
	std::atomic<TTimeDelta> pause_period_;
	std::atomic<TTimePoint> pause_period_start_;
};
