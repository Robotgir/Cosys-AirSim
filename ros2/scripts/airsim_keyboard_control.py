# ready to run example: PythonClient/multirotor/hello_drone.py
import airsim
import numpy as np
import time
import sys
from pynput.keyboard import Key, Listener

client = 0
velocity = 0# m/s

def on_press(key):
    global client

    print('{0} pressed'.format(key))
    duration = 0.5
    veloF = 0.5

    if(key == Key.up):
        client.moveByVelocityAsync(veloF,0,0, duration)
    if(key == Key.down):
        client.moveByVelocityAsync(-veloF,0,0, duration)
    if(key == Key.right):
        client.moveByVelocityAsync(0,veloF,0, duration)
    if(key == Key.left):
        client.moveByVelocityAsync(0,-veloF,0, duration)
    if(key == Key.shift_l):
        client.moveByVelocityAsync(0,0,veloF, duration)
    if(key == Key.ctrl_l):
        client.moveByVelocityAsync(0,0,-veloF, duration)
    if(key == Key.shift_r):
        client.rotateByYawRateAsync(-10, 1)
    if(key == Key.ctrl_r):
        client.rotateByYawRateAsync(10, 1)





def on_release(key):
    global client

    if(key == Key.up):
        client.moveByVelocityAsync(0,0,0,0)
        print(client.getMultirotorState().kinematics_estimated.linear_velocity)

    if(key == Key.down):
        velocity = 0
        client.moveByVelocityAsync(0,0,0,0)
    if(key == Key.right):
        velocity = 0
        client.moveByVelocityAsync(0,0,0,0)
    if(key == Key.left):
        velocity = 0
        client.moveByVelocityAsync(0,0,0,0)
    if(key == Key.shift_l):
        velocity = 0
        client.moveByVelocityAsync(0,0,0,0)
    if(key == Key.ctrl_l):
        velocity = 0
        client.moveByVelocityAsync(0,0,0,0)
    if(key == Key.shift_r):
        client.rotateByYawRateAsync(0, 1)
    if(key == Key.ctrl_r):
        client.rotateByYawRateAsync(0, 1)
        


    if(key == Key.esc):
        # Stop listener
        return False

def hover_height(z):
    global client
    if len(sys.argv) > 1:
        z = float(sys.argv[1])
    print("make sure we are hovering at {} meters...".format(z))
    if z > 5:
        # AirSim uses NED coordinates so negative axis is up.
        # z of -50 is 50 meters above the original launch point.
        client.moveToZAsync(-z, 5).join()
        client.hoverAsync().join()
        time.sleep(5)

    if z > 10:
        print("come down quickly to 10 meters...")
        z = 10
        client.moveToZAsync(-z, 3).join()
        client.hoverAsync().join()



# Collect events until released
with Listener(on_press=on_press, on_release=on_release) as listener:

    # connect to the AirSim simulator
    try:

        client = airsim.MultirotorClient()
        client.confirmConnection()
        client.enableApiControl(True)
        #client.armDisarm(True)
        cinfo = client.simGetCollisionInfo()
        print(cinfo)


        # Async methods returns Future. Call join() to wait for task to complete.
        client.takeoffAsync().join()
        #time.sleep(3)
        #client.moveToPositionAsync(0, 0, -10, 5).join()
        #hover_height(3)
        print("READY FOR INPUTS")
        listener.join() 
    
        

        #moveOnPathAsync([airsim.Vector3r(125,0,z), airsim.Vector3r(125,-130,z), airsim.Vector3r(0,-130,z), airsim.Vector3r(0,0,z)], 12, 120, airsim.DrivetrainType.ForwardOnly, airsim.YawMode(False,0), 20, 1).join()
    except Exception as e:
        print(e, "UNREAL NOT RUNNING YET.")