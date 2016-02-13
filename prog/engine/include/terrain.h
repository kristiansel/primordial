#ifndef TERRAIN_H
#define TERRAIN_H

#include "prop.h"
#include "global.h"
#include "closedmap.h"
#include "physicsworld.h"
#include "quadtree.hpp"

struct TerrainPatch
{
    TerrainPatch();
    TerrainPatch(unsigned int subd_lvl_in);
    void reInit(unsigned int subd_lvl_in) {prop = std::shared_ptr<Prop>(new Prop); mesh = std::shared_ptr<Mesh>(new Mesh); subd_lvl = subd_lvl_in;}
    std::shared_ptr<Prop> prop;
    std::shared_ptr<Mesh> mesh;
    unsigned int subd_lvl;
    int diff_lvl[4];
    // for culling
    QuadAABB world_aabb;
    bool to_render;

};

class Terrain
{
    public:
        Terrain();
        virtual ~Terrain();

        void init(PhysicsWorld* physics_world_in); // Initialize the monolithic "terrain" object

        std::vector<TerrainPatch>* getPatches();

        void updateObserverPosition(glm::vec3 observer_position);

        float ySample(float x, float z) const;
        glm::vec3 normSample(float x, float z);

    protected:
//        std::shared_ptr<Prop> patches_LOD2[20];

        void subdividedQuads(Vertex* &vertices,
                             int &num_verts,
                             Triangle* &triangles,
                             int &num_tris,
                             float half_length,
                             int n_sub,
                             int *lvldiff_nsew,
                             float x_corner,
                             float z_corner);

        void generateHeightMap();

        void generateGraphicsPatch(glm::vec3 center,
                                  float length,
                                  unsigned int num_subd);
        void generatePhysicsPatch(glm::vec3 center,
                                  float length,
                                  int dim);

        void updatePhysicsTerrain(glm::vec3 anchor_in);

        float sampleHeightMap(float x, float z, float length, float center_x, float center_z) const;

        void changeSubdLvl(unsigned int subd_lvl_in, TerrainPatch* terrain_patch, int *lvldiff_nsew);

        int getPrefLvl(float square_distance);

        float getThirdTexCo(const glm::vec3 &normal, const float &height);



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
        unsigned int m_numPatchSubd;
        float m_maxDistSq;

        PhysicsWorld* m_physicsWorld; // owned by physics
        btRigidBody* m_terrainBody; // owned by physics

        unsigned int m_dimPhysHeights;
        float* m_heightData0; // owned by this class, woops, make sure not to leak

        float m_spacingPhysHeights;
        float m_sidePhysHeights;
        glm::vec3 phys_anchor;
        float phys_anchor_l_sq;

        std::vector<TerrainPatch> terrain_patches;

        glm::vec3 anchor_position; // the center of the currently loaded terrain

        unsigned int m_corePatchDim;
};

#endif // TERRAIN_H
