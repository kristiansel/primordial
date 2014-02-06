import io
from math import *
import mathutils
import os
import sys
import time

import bpy
import bpy_extras.io_utils

# NOTES: Meshes have to be triangulated select all faces and 'CTRL - T'
#          model ends up in:     Blender Foundation\Blender\
#        it now works! yes!
#        mesh.name must be object.name + .some number, for example: 
#                                                               mesh.name = 'Plane.003'
#                                                               object.name = 'Plane'

def pc(sin):
    print('')
    print(sin)
    
def write_obj(folder):
    if not os.path.isdir(folder):
        os.mkdir(folder)
        
    for mesh in bpy.data.meshes:
        
        object_name = 'Plane'
        
        obj = bpy.data.objects[object_name]
#        rx = obj.rotation_euler.x
#        ry = obj.rotation_euler.y
#        rz = obj.rotation_euler.z
        rm = obj.rotation_euler.to_matrix()
        
        
        out = io.FileIO(folder+'/'+mesh.name+'.obj', 'w')      
        for vert in mesh.vertices:
            
            x = (vert.co.x)
            y = (vert.co.y)
            z = (vert.co.z)
            
             
            #rotate this is wrong. maybe the convension is wrong MAYBE, BUT X NEED TEMPORARY X Y AND Z
#            x = (cos(rz)*cos(rx)-cos(ry)*sin(rx)*sin(rz))*x + (cos(rz)*sin(rx)+cos(ry)*cos(rx)*sin(rz))*y + (sin(rz)*sin(ry))*z
#            y = (-sin(rz)*cos(rx)-cos(ry)*sin(rx)*cos(rz))*x + (-sin(rz)*-sin(rx)+cos(ry)*cos(rx)*cos(rz))*y + (cos(rz)*sin(ry))*z
#            z = (sin(ry)*sin(rx))*x + (-sin(ry)*cos(rx))*y + (cos(ry))*z

            xt = rm[0][0] * x + rm[0][1] * y + rm[0][2] * z
            yt = rm[1][0] * x + rm[1][1] * y + rm[1][2] * z
            zt = rm[2][0] * x + rm[2][1] * y + rm[2][2] * z
            
            x = xt
            y = yt
            z = zt
                      
            #scale
            x = obj.scale.x * x
            y = obj.scale.y * y
            z = obj.scale.z * z
            
            #translate
            x = x + obj.location.x
            y = y + obj.location.y
            z = z + obj.location.z
            
            
            out.write(bytes('v %f %f %f\n' % (x, y, z), 'UTF-8'))
        for vert in mesh.vertices:
            out.write(bytes('vn %f %f %f\n' % (vert.normal.x, vert.normal.y, vert.normal.z), 'UTF-8'))
        for face in mesh.polygons:
            out.write(bytes('f', 'UTF-8'))
            
            for vert in face.vertices:
                out.write(bytes(' %i' % (vert+1), 'UTF-8' ))
            out.write('\n'.encode('utf-8'))
        for vert in mesh.vertices:
            out.write(bytes('w', 'UTF-8'))
            
            for group in vert.groups:
                # if (bpy.data.objects[object_name].vertex_groups[group.group].name!='tumb`_R'):
                out.write(bytes(' (%s:' % (bpy.data.objects[object_name].vertex_groups[group.group].name), 'UTF-8' ))
                out.write(bytes('%f)' % (group.weight), 'UTF-8' ))
                
            out.write('\n'.encode('utf-8'))
           
        out.close()

pc('-----running script')

# armature = bpy.data.objects['Armature']
# anim_data = armature.animation_data

dir()

write_obj('D:/Travel/Productivity/blender/output/exported_models')

#pc(list(bpy.data.objects))
#pc(dir(anim_data))
#pc(dir(anim_data.action))
#pc(dir(anim_data.action.pose_markers))

pc('-----finished script!')
pc('')