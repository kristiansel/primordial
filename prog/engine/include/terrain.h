#ifndef TERRAIN_H
#define TERRAIN_H

#include "prop.h"
#include "global.h"
#include "closedmap.h"
#include "physicsworld.h"

class Terrain
{
    public:
        Terrain();
        virtual ~Terrain();

        void init(PhysicsWorld* physics_world_in); // Initialize the monolithic "terrain" object

        std::vector<std::shared_ptr<Prop>>* getPatches();

        void updateObserverPosition(glm::vec3 observer_position);

    protected:
//        std::shared_ptr<Prop> patches_LOD2[20];

        void subdividedQuads(Vertex* &vertices,
                             int &num_verts,
                             Triangle* &triangles,
                             int &num_tris,
                             float half_length,
                             int n_sub,
                             float x_corner,
                             float z_corner);

        void generateHeightMap();

        void generateGraphicsPatch(glm::vec3 center,
                                  float length,
                                  int dim);
        void generatePhysicsPatch(glm::vec3 center,
                                  float length,
                                  int dim);

        void updatePhysicsTerrain(glm::vec3 anchor_in);

        float sampleHeightMap(float x, float z, float length, float center_x, float center_z);



    private:
        int m_dimension;
        //float height_map[map_dim*map_dim];
        //int lin_i(int i, int j);

        ClosedMap<float> m_height_map;

        float m_heightScale;
        float m_horzScale;
        float m_centerX;
        float m_centerZ;

        float m_patchLength;

        PhysicsWorld* m_physicsWorld;
        btRigidBody* m_terrainBody;

        unsigned int m_dimPhysHeights;
        float* m_heightData0;

        float m_spacingPhysHeights;
        float m_sidePhysHeights;
        //float* m_heightData1;

        std::vector<std::shared_ptr<Prop>> terrain_patches; // Keep own geometry since none of it is duplicate
        std::vector<std::shared_ptr<Mesh>> terrain_meshes;

        glm::vec3 anchor_position; // the center of the currently loaded terrain

};

#endif // TERRAIN_H
