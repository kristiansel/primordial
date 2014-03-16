import bpy

# hard-coded folder to export to
folder = "D:/Travel/Productivity/blender/projects/human_m/animations_all_2"

# hard-coded object to change action of
anim_object = bpy.data.objects['Scene_root']

# loop through all actions
for curr_action in bpy.data.actions:
    # construct a filename corresponding to action
    save_path = folder+"/"+curr_action.name+".dae"
    
    # change to current action
    anim_object.animation_data.action = curr_action
    
    # export collada
    bpy.ops.wm.collada_export(filepath=save_path)

# finished
    
    
