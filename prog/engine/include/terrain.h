#ifndef TERRAIN_H
#define TERRAIN_H

#include "prop.h"
#include "global.h"
#include "closedmap.h"
#include "physicsworld.h"
#include "quadtree.hpp"
#include "threadingwrapper.h"

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

        // init should be MADE thread safe
        void init(PhysicsWorld* physics_world_in); // Initialize the monolithic "terrain" object

        std::vector<TerrainPatch>* getPatches(); // This function itself is assumed thread safe

        void updateObserverPosition(glm::vec3 observer_position); // This NEEDS to be thread safe

        float ySample(float x, float z) const; // So  does this
        glm::vec3 normSample(float x, float z); // and this

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

        template<typename T>
        static T bilinear(
        T tx,
        T ty,
        T c00,
        T c10,
        T c01,
        T c11)
        {
            T a = c00 * (T(1) - tx) + c10 * tx;
            T b = c01 * (T(1) - tx) + c11 * tx;
            return a * (T(1) - ty) + b * ty;
        };

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

        PhysicsWorld* m_physicsWorld;
        btRigidBody* m_terrainBody;

        unsigned int m_dimPhysHeights;
        float* m_heightData0;

        float m_spacingPhysHeights;
        float m_sidePhysHeights;
        glm::vec3 phys_anchor;
        float phys_anchor_l_sq;

        // Shared resource                               Mutex
        std::vector<TerrainPatch> terrain_patches;       public: PrimT::Mutex patch_mx;
    private:
        glm::vec3 anchor_position; // the center of the currently loaded terrain

        unsigned int m_corePatchDim;

};

//double cubicInterpolate (double p[4], double x) {
//	return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
//}
//
//double bicubicInterpolate (double p[4][4], double x, double y) {
//	double arr[4];
//	arr[0] = cubicInterpolate(p[0], y);
//	arr[1] = cubicInterpolate(p[1], y);
//	arr[2] = cubicInterpolate(p[2], y);
//	arr[3] = cubicInterpolate(p[3], y);
//	return cubicInterpolate(arr, x);
//}
//
//double tricubicInterpolate (double p[4][4][4], double x, double y, double z) {
//	double arr[4];
//	arr[0] = bicubicInterpolate(p[0], y, z);
//	arr[1] = bicubicInterpolate(p[1], y, z);
//	arr[2] = bicubicInterpolate(p[2], y, z);
//	arr[3] = bicubicInterpolate(p[3], y, z);
//	return cubicInterpolate(arr, x);
//}
//
//double nCubicInterpolate (int n, double* p, double coordinates[]) {
//	assert(n > 0);
//	if (n == 1) {
//		return cubicInterpolate(p, *coordinates);
//	}
//	else {
//		double arr[4];
//		int skip = 1 << (n - 1) * 2;
//		arr[0] = nCubicInterpolate(n - 1, p, coordinates + 1);
//		arr[1] = nCubicInterpolate(n - 1, p + skip, coordinates + 1);
//		arr[2] = nCubicInterpolate(n - 1, p + 2*skip, coordinates + 1);
//		arr[3] = nCubicInterpolate(n - 1, p + 3*skip, coordinates + 1);
//		return cubicInterpolate(arr, *coordinates);
//	}
//}

#endif // TERRAIN_H
