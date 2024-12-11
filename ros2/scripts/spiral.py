import airsim
import math
import time

# Connect to the AirSim client
client = airsim.MultirotorClient()
client.confirmConnection()
client.enableApiControl(True)
client.armDisarm(True)

# Takeoff and move to a starting altitude of 25m
altitude = 30
client.takeoffAsync().join()
client.moveToZAsync(-altitude, 3).join()  # AirSim uses negative Z for altitude

# Save the starting position
start_position = client.getMultirotorState().kinematics_estimated.position

# Spiral parameters
radius_increment = 1  # Increase radius by 1m each revolution
angle_step = 10       # Degrees to move per step
spiral_revolutions = 3
current_radius = 0

print("Starting spiral pattern...")
for revolution in range(spiral_revolutions):
    for angle_deg in range(0, 360, angle_step):
        angle_rad = math.radians(angle_deg)
        current_radius += radius_increment / (360 / angle_step) if angle_deg == 0 else 0
        x = start_position.x_val + current_radius * math.cos(angle_rad)
        y = start_position.y_val + current_radius * math.sin(angle_rad)
        z = -altitude
        client.moveToPositionAsync(x, y, z, 5).join()
        time.sleep(0.1)  # Short delay for smoother motion

print("Spiral pattern complete. Returning to start position...")
client.moveToPositionAsync(start_position.x_val, start_position.y_val, -altitude, 10).join()

# Land and disarm
client.landAsync().join()
client.armDisarm(False)
client.enableApiControl(False)

print("Mission completed!")
