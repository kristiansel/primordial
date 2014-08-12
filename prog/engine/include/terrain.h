#ifndef TERRAIN_H
#define TERRAIN_H

#include "prop.h"
#include "global.h"
#include "closedmap.h"
#include "physicsworld.h"

#define HEIGHTMAP_TERRAIN_PHYSICS
//#define TRIANGLE_TERRAIN_PHYSICS

class Terrain
{
    public:
        Terrain();
        virtual ~Terrain();

        void init(PhysicsWorld* physics_world_in); // Initialize the monolithic "terrain" object

        std::vector<std::shared_ptr<Prop>>* getPatches();

    protected:
        std::vector<std::shared_ptr<Prop>> terrain_patches; // Keep own geometry since none of it is duplicate
        std::vector<std::shared_ptr<Mesh>> terrain_meshes;

        std::shared_ptr<Prop> patches_LOD1[9]; // 3*3 with LOD 1
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
        void generatePhysicsPatch(glm::vec3 center,
                                  float length,
                                  int dim);

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

        PhysicsWorld* m_physicsWorld;

        float* m_heightData;

//        unsigned int m_numVertices;
//        unsigned int m_numVertices;
//
//        Vertex* m_terrainVertices;
//        Triangle* m_triangleVertices;

};

#endif // TERRAIN_H
