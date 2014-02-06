import bpy as bpy

bpy.ops.nla.bake(frame_start=bpy.context.scene.frame_start, 
				 frame_end=bpy.context.scene.frame_end, 
				 step=1, only_selected=False)