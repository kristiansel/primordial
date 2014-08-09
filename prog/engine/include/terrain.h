#ifndef TERRAIN_H
#define TERRAIN_H

#include "prop.h"
#include "global.h"
#include "closedmap.h"

class Terrain
{
    public:
        Terrain();
        virtual ~Terrain();

        void init(); // Initialize the monolithic "terrain" object

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

        float sampleHeightMap(float x, float z, float length, float center_x, float center_z);

    private:
        int m_dimension;
        //float height_map[map_dim*map_dim];
        //int lin_i(int i, int j);

        ClosedMap<float> m_height_map;
};

#endif // TERRAIN_H
