import bpy
import math
import sys
import os
import json


def getTuple(json, angle = False, default = 0):
	if json == None:
		return (default, default, default)

	if angle:
		return (math.radians(json[0]), math.radians(json[1]), math.radians(json[2]))
	
	return tuple(json)

def getNumber(json):
	if json == None:
		return 0
	
	return json

if __name__ == "__main__":
	argv = sys.argv
	argv = argv[argv.index("--") + 1:] 

	patternPath = os.path.abspath(argv[0])
	capturePath = os.path.abspath(argv[1])
	outputPath = os.path.abspath(argv[2])
	cameraWidth = int(argv[3])
	cameraHeight = int(argv[4])
	cameraHorizontalFOV = float(argv[5])
	projectorHorizontalFOV = float(argv[6])
	halfCameraProjectorSeparation = float(argv[7]) / 2.0
	saveBlenderFile = (argv[8] == "true")

	jsonData = json.loads(open(os.path.abspath("./slVirtualScene.json")).read())

	bpy.ops.scene.new(type='EMPTY')
        	
	for sceneObjectJson in jsonData['objects']:
		objectType = sceneObjectJson.get('type')

		location = getTuple(sceneObjectJson.get('location'))
		rotation = getTuple(sceneObjectJson.get('rotation'), True)

		sceneObject = None

		if objectType == "plane":
			bpy.ops.mesh.primitive_plane_add(location=location, rotation=rotation)
		elif objectType == "sphere":
			bpy.ops.mesh.primitive_uv_sphere_add(location=location, rotation=rotation)
		elif objectType == "cube":
			bpy.ops.mesh.primitive_cube_add(location=location, rotation=rotation)
		
		sceneObject = bpy.context.object
		sceneObjectData = sceneObject.data

		sceneObject.scale = getTuple(sceneObjectJson.get('scale'), default=1)
		#sceneObject.rotation_euler = getTuple(sceneObjectJson.get('rotation_euler'), True)


	#bpy.ops.import_scene.obj(filepath="/Users/ehendrikd/Downloads/GI Joe - The Rock/TheRock2.obj")
	#obj = bpy.context.object
	#obj.scale = (0.2, 0.2, 0.2)

	radians90 = math.radians(90)
        
	bpy.ops.object.lamp_add(type='SPOT', location=(0, halfCameraProjectorSeparation, 0), rotation=(0, radians90, 0))


	projectorObject = bpy.context.object
	projectorObject.delta_rotation_euler = (radians90, 0, 0)

	projector = projectorObject.data

	#projector.distance = 10
	projector.spot_size = math.radians(projectorHorizontalFOV)
	projector.use_square = True
	projector.show_cone = True
	projector.energy = 500
	#projector.falloff_type = 'CONSTANT'
	projector.spot_blend = 0
	projector.shadow_buffer_clip_end = 1000

	image = bpy.data.images.load(patternPath)
	texture = bpy.data.textures.new('ColorTex', type = 'IMAGE')
	texture.image = image   
	texture.extension = 'CLIP' 
	projector.active_texture = texture
	projector.texture_slots[0].texture_coords = 'VIEW'

	#print(type(cTex))

	#realpath = os.path.expanduser('~/snippets/textures/color.png')
	#try:
	#    img = bpy.data.images.load(realpath)
	#except:
	#    raise NameError("Cannot load image %s" % realpath)

	bpy.ops.object.camera_add(location=(0, -halfCameraProjectorSeparation, 0), rotation=(radians90, 0, radians90))
	camera = bpy.context.object
	camera.data.angle_x = math.radians(cameraHorizontalFOV)
	#camera.data.type = 'ORTHO'
	#camera.data.ortho_scale = 8.69
	bpy.context.scene.camera = camera


	bpy.context.scene.render.resolution_x = cameraWidth
	bpy.context.scene.render.resolution_y = cameraHeight
	bpy.context.scene.render.resolution_percentage = 100

	bpy.context.scene.render.filepath = capturePath

	if saveBlenderFile:
		bpy.ops.wm.save_as_mainfile(filepath=outputPath)

	bpy.ops.render.render( write_still=True ) 

	
