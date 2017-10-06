import bpy
import math
import sys
import os
import json
import numpy as np
from mathutils import Vector

def getDirection(camera):
	#Y = np.tan(np.deg2rad(camera.angle_x / 2))
	#Z = np.tan(np.deg2rad(camera.angle_y / 2))
	Y = np.tan(bpy.data.cameras['Camera.001'].angle_x / 2)
	Z = np.tan(bpy.data.cameras['Camera.001'].angle_y / 2)

	return (-1, Y, Z)

def getDirections(obj, camera, cameraWidth, cameraHeight, outputPath):
	out = open(outputPath, 'w')
	x, y, z = getDirection(camera)

	for yDirection in np.linspace(-y, y, cameraWidth):
		for zDirection in np.linspace(z, -z, cameraHeight):
			dst = Vector((x, yDirection, zDirection)) + camera.location
			direction = Vector((x, yDirection, zDirection)) 


			mw = obj.matrix_world
			mwi = mw.inverted()

			# src and dst in local space of cb

			origin = mwi * camera.location
			dest = mwi * dst
			direction = (dest - origin).normalized()

			result, location, normal, index = obj.ray_cast(origin,  direction)

			#xPixel = int(round(((yDirection + y) / (2 * y)) * cameraWidth))
			#yPixel = int(round(((-zDirection + z) / (2 * z)) * cameraHeight))
			xPixel = ((yDirection + y) / (2 * y)) * cameraWidth
			yPixel = ((-zDirection + z) / (2 * z)) * cameraHeight
			locationWorld = mw * location
			fromCamera = locationWorld - camera.location

			if result:
				out.write(str(xPixel) + " " + str(yPixel) + " " + str(fromCamera.x) + '\n')
	out.close()

if __name__ == "__main__":
	argv = sys.argv
	argv = argv[argv.index("--") + 1:] 

	bpy.ops.wm.open_mainfile(filepath=os.path.abspath(argv[0]))

	outputPath = os.path.abspath(argv[1])
	cameraWidth = int(argv[2])
	cameraHeight = int(argv[3])

	singleObject = None

	for ob in bpy.context.scene.objects:
		if ob.type == 'MESH':
			ob.select = True
			bpy.context.scene.objects.active = ob			
		else:
			ob.select = False

	bpy.ops.object.join()

	for ob in bpy.context.scene.objects:
		if ob.type == 'MESH':
			singleObject = ob


	getDirections(singleObject, bpy.context.scene.camera, cameraWidth, cameraHeight, outputPath)

	
