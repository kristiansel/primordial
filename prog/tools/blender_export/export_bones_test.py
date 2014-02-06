import io
import math
import mathutils
import os
import sys
import time

import bpy
import bpy_extras.io_utils

# NOTES: model ends up in: 
#        it now works! yes!
#        Animation key frames stored under 'bpy.data.actions'
#        armature must be called 'Armature'
#        now, bone weights are needed


def pc(sin):
    print('')
    print(sin)
    
def write_file(folder):
    
    vArmatureName = 'MyArmature'  
    vActionName = 'wave1'
    
    if not os.path.isdir(folder):
        os.mkdir(folder)
    
    for armature in bpy.data.armatures:
        
        object_name = ''        
        if '.' in armature.name:
            ok = False
            i = 0
            while ok==False:
                if armature.name[i+1]=='.':
                    ok = True  
                object_name = object_name + armature.name[i]
                i = i+1
        else:
            object_name = armature.name
        
        obj = bpy.data.objects[vArmatureName]
        rm = obj.rotation_euler.to_matrix()
        
        
        
        
        out = io.FileIO(folder+'/'+armature.name+'.bns', 'w')
        
        bones = armature.bones
    
        out.write(bytes('bones \n', 'UTF-8'))
       
        for bone in bones:
            out.write(bytes('   bone %s \n' % bone.name, 'UTF-8'))
            print(type(bone.parent))
            if bone.parent is None:
                out.write(bytes('       parent None \n', 'UTF-8'))
            else:
                out.write(bytes('       parent %s \n' % bone.parent.name, 'UTF-8'))
                
            out.write(bytes('       children ', 'UTF-8'))
           
            for child in bone.children:
                out.write(bytes('%s ' % child.name, 'UTF-8'))
            if not bone.children:
                out.write(bytes('None', 'UTF-8'))
            out.write(bytes('\n', 'UTF-8'))
            
            hx = (bone.head_local.x)
            hy = (bone.head_local.y)
            hz = (bone.head_local.z)
            
            #scale
            hx = obj.scale.x * hx
            hy = obj.scale.y * hy
            hz = obj.scale.z * hz
            
            #rotate
            hxt = rm[0][0] * hx + rm[0][1] * hy + rm[0][2] * hz
            hyt = rm[1][0] * hx + rm[1][1] * hy + rm[1][2] * hz
            hzt = rm[2][0] * hx + rm[2][1] * hy + rm[2][2] * hz
            
            hx = hxt
            hy = hyt
            hz = hzt
            
            #translate
            hx = hx + obj.location.x
            hy = hy + obj.location.y
            hz = hz + obj.location.z

            tx = (bone.tail_local.x)
            ty = (bone.tail_local.y)
            tz = (bone.tail_local.z)
            
            #scale think this should come after rotation?
            tx = obj.scale.x * tx
            ty = obj.scale.y * ty
            tz = obj.scale.z * tz
            
            #rotate
            hxt = rm[0][0] * tx + rm[0][1] * ty + rm[0][2] * tz
            hyt = rm[1][0] * tx + rm[1][1] * ty + rm[1][2] * tz
            hzt = rm[2][0] * tx + rm[2][1] * ty + rm[2][2] * tz
            
            tx = hxt
            ty = hyt
            tz = hzt
            
            #translate
            tx = tx + obj.location.x
            ty = ty + obj.location.y
            tz = tz + obj.location.z



            out.write(bytes('       head %f %f %f \n' % (hx, hy, hz) , 'UTF-8'))
            out.write(bytes('       tail %f %f %f \n' % (tx, ty, tz) , 'UTF-8'))
            
#            out.write(bytes('       roll %f \n' % (bone.roll*180/3.14159265) , 'UTF-8'))
            
            mat = bone.matrix
            
            out.write(bytes('       rotation_rest %f %f %f \n' % (mat[0][0], mat[0][1], mat[0][2]) , 'UTF-8'))
            out.write(bytes('                     %f %f %f \n' % (mat[1][0], mat[1][1], mat[1][2]) , 'UTF-8'))
            out.write(bytes('                     %f %f %f \n' % (mat[2][0], mat[2][1], mat[2][2]) , 'UTF-8'))
        
        # FINISHED WRITING BONES
        #out.write(bytes('\n', 'UTF-8'))
        # NOW ON TO WRITING FRAME DATA
        
        # groups correspond to bones
        # fcurves correspond to bones.channels-1
        
        channel_names = ('x_locat', 'y_locat', 'z_locat', 'w_q_rot', 'x_q_rot', 'y_q_rot', 'z_q_rot', 'x_scale', 'y_scale', 'z_scale')
        
        actions = bpy.data.actions[vActionName]
        
        out.write(bytes('animation\n', 'UTF-8'))
        out.write(bytes('   frame_range %f %f\n' % (actions.frame_range.x, actions.frame_range.y), 'UTF-8'))
        
        for bone_anim in actions.groups:
            out.write(bytes('   bone_anim %s\n' % (bone_anim.name), 'UTF-8'))
                
            for i in range(0, len(bone_anim.channels)):
                if (bone_anim.name=='spine1'):
                    print(len(bone_anim.channels[i].keyframe_points))
                    
                out.write(bytes('       %s ' % (channel_names[i]), 'UTF-8'))
                for key_frame in bone_anim.channels[i].keyframe_points:
                    if (i<3): # a translationbly.
                        out.write(bytes('(%f:%f) ' % (key_frame.co.x, key_frame.co.y*obj.scale[i]), 'UTF-8')) # obj.scale[i]*
                    else:
                        out.write(bytes('(%f:%f) ' % (key_frame.co.x, key_frame.co.y), 'UTF-8'))
                    # out.write(bytes('(%f:%f) ' % (key_frame.co.x, key_frame.co.y), 'UTF-8'))
                out.write(bytes('\n', 'UTF-8'))
                
                
        
        out.close()

pc('-----running script')

write_file('D:/Travel/Productivity/blender/output/exported_animations')

pc('-----finished script!')
pc('')