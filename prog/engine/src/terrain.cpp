#include "terrain.h"

Terrain::Terrain() :
    m_dimension(256),   // 256, 2048
    m_height_map(256),  // 256, 2048
    m_heightScale(0.0100), // 0.016, 5*0.016
    m_horzScale(256), // 1024, 4096
    m_centerX(0.0), // 0.0
    m_centerZ(0.0), // 0.0
    m_patchLength(300), // 32, 96
    m_numPatchSubd(7),
    m_physicsWorld(nullptr),
    m_terrainBody(nullptr),
    m_heightData0(nullptr),
    phys_anchor_l_sq(1024), // physics moves when distance to anchor = 32 m
    m_corePatchDim(8) // 4, 32
{
    //ctor
    //m_sqLODthresh = {}
    m_maxDistSq = pow((m_corePatchDim/2.f*m_patchLength)*0.9, 2.0);
}

Terrain::~Terrain()
{
    //dtor
    if (m_heightData0) delete [] m_heightData0; // how does this check out ordering-wise?
}

TerrainPatch::TerrainPatch() :
    diff_lvl{-100, -100, -100, -100}
{
    prop = std::shared_ptr<Prop>(new Prop);
    mesh = std::shared_ptr<Mesh>(new Mesh);
    subd_lvl = 0;
}

TerrainPatch::TerrainPatch(unsigned int subd_lvl_in) :
    diff_lvl{-100, -100, -100, -100}
{
    prop = std::shared_ptr<Prop>(new Prop);
    mesh = std::shared_ptr<Mesh>(new Mesh);
    subd_lvl = subd_lvl_in;
}

void Terrain::init(PhysicsWorld* physics_world_in /*, glm::vec3 position_terr*/)
{
    std::cout << "initializing terrain...\n";

    m_physicsWorld = physics_world_in;

    generateHeightMap();

    anchor_position = glm::vec3(0.0, 0.0, 0.0);
    phys_anchor = glm::vec3(0.0, 0.0, 0.0);

    for (unsigned int i = 0; i<m_corePatchDim; i++)
    {
        for (unsigned int j = 0; j<m_corePatchDim; j++)
        {
            float start_offset = (float)(m_corePatchDim-1)/2.0;

            float center_x = -start_offset*m_patchLength + j*m_patchLength;
            float center_z = -start_offset*m_patchLength + i*m_patchLength;

            //generateGraphicsPatch(glm::vec3(center_x, 0.0, center_z), m_patchLength, m_numPatchSubd);
            generateGraphicsPatch(glm::vec3(center_x, 0.0, center_z), m_patchLength, 0);
        }
    }

    generatePhysicsPatch(glm::vec3(0.0, 0.0, 0.0), 2*m_patchLength, 2*pow(2, m_numPatchSubd));

    std::cout << "length of terrain_patches: " << terrain_patches.size() << "\n";
}

std::vector<TerrainPatch>* Terrain::getPatches()
{
    return &terrain_patches;
}

float Terrain::getThirdTexCo(const glm::vec3 &normal, const float &height)
{
    float horz_length = pow(glm::length(glm::vec2(normal.x, normal.z)), 0.7f); // 0...1.0

    float height_comp;
    float height_low = 0.0;
    float transition_length = 10.0;
    if (height > height_low)
    {
        if ((height < (height_low+transition_length)))
        {
            height_comp = (height-height_low)/transition_length;
        }
        else
        {
            height_comp = 1.0;
        }
    }
    else
    {
        height_comp = 0.0;
    }

    float out = std::max(horz_length, height_comp);

    //std::cout << "returning"
    return out;
}

void Terrain::subdividedQuads(Vertex* &vertices,
                              int &num_verts,
                              Triangle* &triangles,
                              int &num_tris,
                              float half_length,
                              int n_sub,
                              int *lvldiff_nsew,
                              float x_corner,
                              float z_corner)
{

    int n_side = pow(2, n_sub);
    int n_quads = n_side*n_side;

    int verts_per_quad = 4;
    int tris_per_quad = 2;

    num_verts = verts_per_quad * n_quads;
    num_tris = tris_per_quad * n_quads;

    vertices = new Vertex [num_verts];
    triangles = new Triangle [num_tris];

    float texcoord_stride = 18.0/(float)(n_side);

    float quad_length = 2*half_length/n_side;

    for (int i = 0; i<n_side; i++)
    {
        for (int j = 0; j<n_side; j++)
        {
            //     1-------2    A
            //     | _0___/|    | -z
            //     |/   1  |    |
            //     0-------3    y----> X
            int first_vert_index = verts_per_quad*(i*n_side + j);
            int first_tri_index = tris_per_quad*(i*n_side + j);

            Vertex* verts = &vertices[first_vert_index];
            Triangle* tris = &triangles[first_tri_index];

            float x = -half_length + j*quad_length;
            float z = -half_length + i*quad_length;

            float sample0 = m_heightScale*sampleHeightMap(x+x_corner, z+quad_length+z_corner, m_horzScale, m_centerX, m_centerZ);
            float sample1 = m_heightScale*sampleHeightMap(x+x_corner, z+z_corner, m_horzScale, m_centerX, m_centerZ);
            float sample2 = m_heightScale*sampleHeightMap(x+quad_length+x_corner, z+z_corner, m_horzScale, m_centerX, m_centerZ);
            float sample3 = m_heightScale*sampleHeightMap(x+quad_length+x_corner, z+quad_length+z_corner, m_horzScale, m_centerX, m_centerZ);

            if (i<0 && j<0)
            {
                // std::cout << "samples " << sample0 << ", " << sample1 << ", " << sample2 << ", " << sample3 << "\n";
                std::cout << "samples "<< x+x_corner <<", " <<z+z_corner << ": " << sample1 << "\n";
            }


            verts[0].position = glm::vec4(x, sample0, z+quad_length, 1.0);
            verts[1].position = glm::vec4(x, sample1, z, 1.0);
            verts[2].position = glm::vec4(x+quad_length, sample2, z, 1.0);
            verts[3].position = glm::vec4(x+quad_length, sample3, z+quad_length, 1.0);
//
//            glm::vec3 facenormal = glm::normalize(glm::cross(glm::vec3(verts[3].position.x, verts[3].position.y, verts[3].position.z)-
//                                                             glm::vec3(verts[0].position.x, verts[0].position.y, verts[0].position.z),
//                                                             glm::vec3(verts[1].position.x, verts[1].position.y, verts[1].position.z)-
//                                                             glm::vec3(verts[0].position.x, verts[0].position.y, verts[0].position.z)));
//
//
//            verts[0].normal = facenormal;
//            verts[1].normal = facenormal;
//            verts[2].normal = facenormal;
//            verts[3].normal = facenormal;

            // make a numerical estimation of the normal
            verts[0].normal = normSample(x+x_corner, z+quad_length+z_corner);
            verts[1].normal = normSample(x+x_corner, z+z_corner);
            verts[2].normal = normSample(x+quad_length+x_corner, z+z_corner);
            verts[3].normal = normSample(x+quad_length+x_corner, z+quad_length+z_corner);


            // stretch one texture over one terrain quad

            verts[0].tex_coords[0] = j*texcoord_stride; verts[0].tex_coords[1] = (i+1)*texcoord_stride; verts[0].tex_coords[2] = getThirdTexCo(verts[0].normal, verts[0].position.y);
            verts[1].tex_coords[0] = j*texcoord_stride; verts[1].tex_coords[1] = (i)*texcoord_stride; verts[1].tex_coords[2] = getThirdTexCo(verts[1].normal, verts[1].position.y);
            verts[2].tex_coords[0] = (j+1)*texcoord_stride; verts[2].tex_coords[1] = (i)*texcoord_stride; verts[2].tex_coords[2] = getThirdTexCo(verts[2].normal, verts[2].position.y);
            verts[3].tex_coords[0] = (j+1)*texcoord_stride; verts[3].tex_coords[1] = (i+1)*texcoord_stride; verts[3].tex_coords[2] = getThirdTexCo(verts[3].normal, verts[3].position.y);

//            verts[0].tex_coords[0] = 0.0; verts[0].tex_coords[1] = 0.0; verts[0].tex_coords[2] = getThirdTexCo(verts[0].normal, verts[0].position.y);
//            verts[1].tex_coords[0] = 0.0; verts[1].tex_coords[1] = 1.0; verts[1].tex_coords[2] = getThirdTexCo(verts[1].normal, verts[1].position.y);
//            verts[2].tex_coords[0] = 1.0; verts[2].tex_coords[1] = 1.0; verts[2].tex_coords[2] = getThirdTexCo(verts[2].normal, verts[2].position.y);
//            verts[3].tex_coords[0] = 1.0; verts[3].tex_coords[1] = 0.0; verts[3].tex_coords[2] = getThirdTexCo(verts[3].normal, verts[3].position.y);

            for (unsigned int i = 0; i<4; i++)
            {
                for (unsigned int n_bon = 0; n_bon<MAX_BONE_INFLUENCES; n_bon++)
                {
                    verts[i].bone_indices[n_bon] = 0;
                    if (n_bon == 0)
                    {
                        verts[i].bone_weights[n_bon] = 1;
                    }
                    else
                    {
                        verts[i].bone_weights[n_bon] = 0;
                    }

                }
            }


            tris[0].indices[0] = first_vert_index + 0;
            tris[0].indices[1] = first_vert_index + 2;
            tris[0].indices[2] = first_vert_index + 1;

            tris[1].indices[0] = first_vert_index + 0;
            tris[1].indices[1] = first_vert_index + 3;
            tris[1].indices[2] = first_vert_index + 2;
        }
    }

    // stick edge vertices to neighboring patches of lower detail
    /**if (lvldiff_nsew[0] < 0)
    {
        int skip = std::abs(lvldiff_nsew[0]);
        int diff_samepos = 3;
        int diff_skip = 1+pow(2,skip+1)
        for (int start = 2; j<n_side*4; j++)
        {
            //          1--2 1--2 1--2 1--2 1--2 1--2 1--2 1--2 1--2 1--2 1--2
            // -1 :     x  ( )   x   ( )   x   ( )   x   ( )   x   ( )
            //             2 5      10 13     18 21     26 29
            // -2 :     x  ( )  ( )  ( )   x   ( )  ( )  ( )   x
            // -3 :     x  ( )  ( )  ( )  ( )  ( )  ( )  ( )   x   ( )

        }
    }*/

    // new algorithm:
    // fill interior first, then edges

    if (lvldiff_nsew[0] < 0)
    {
        int skip = pow(2, std::abs(lvldiff_nsew[0])); // 2, 4, 8 etc

        int corner_offset = 1;
        int skip_vert_stride = skip*verts_per_quad;

        float left_height = vertices[corner_offset].position.y;
        float right_height = vertices[corner_offset + skip_vert_stride-3].position.y;

        int i = 0;
        for (int j = 0; j<n_side-1; j++) // all columns
        {
            int first_vert_index = verts_per_quad*(i*n_side + j);
            Vertex* verts = &(vertices[first_vert_index]);

            int edge_num = j+1;
            int mod = edge_num%skip;

            if (mod==0)
            {
                //update interpolants
                left_height = verts[2].position.y;
                right_height = verts[2 + skip_vert_stride].position.y;
            }
            else
            {
                float left_weight = 1.0-(float)(mod)/(float)(skip);
                float right_weight = (float)(mod)/(float)(skip);

                verts[2].position.y = left_height * left_weight +
                                      right_height * right_weight;
                verts[2+3].position.y = verts[2].position.y;
            }
        } // for
    } // fix north edge


    if (lvldiff_nsew[1] < 0)
    {
        int skip = pow(2, std::abs(lvldiff_nsew[1])); // 2, 4, 8 etc

        int corner_offset = verts_per_quad*(n_side-1)*n_side; // +first vertex of last row
        int skip_vert_stride = skip*verts_per_quad;

        float left_height = vertices[corner_offset].position.y;
        float right_height = vertices[corner_offset + skip_vert_stride-1].position.y;

        int i = (n_side-1);
        for (int j = 0; j<n_side-1; j++) // all columns
        {
            int first_vert_index = verts_per_quad*(i*n_side + j);
            Vertex* verts = &(vertices[first_vert_index]);

            int edge_num = j+1;
            int mod = edge_num%skip;

            if (mod==0)
            {
                //update interpolants
                left_height = verts[3].position.y;
                right_height = verts[3 + skip_vert_stride].position.y;
            }
            else
            {
                float left_weight = 1.0-(float)(mod)/(float)(skip);
                float right_weight = (float)(mod)/(float)(skip);

                verts[3].position.y = left_height * left_weight +
                                      right_height * right_weight;
                verts[3+1].position.y = verts[3].position.y; // overlapping vertex in the next quad
            }
        } // for
    } // fix south edge

    if (lvldiff_nsew[2] < 0)
    {
        int skip = pow(2, std::abs(lvldiff_nsew[2])); // 2, 4, 8 etc

        int corner_offset = verts_per_quad*(n_side-1)+2; // +first vertex of last column
        int skip_vert_stride = skip*(verts_per_quad*n_side);

        float left_height = vertices[corner_offset].position.y;
        float right_height = vertices[corner_offset + skip_vert_stride-verts_per_quad*n_side+1].position.y;

        int j = (n_side-1);
        for (int i = 0; i<n_side-1; i++) // all rows
        {
            int first_vert_index = verts_per_quad*(i*n_side + j);
            Vertex* verts = &(vertices[first_vert_index]);

            int edge_num = i+1;
            int mod = edge_num%skip;

            if (mod==0)
            {
                //update interpolants
                left_height = verts[3].position.y;
                right_height = verts[3 + skip_vert_stride].position.y;
            }
            else
            {
                float left_weight = 1.0-(float)(mod)/(float)(skip);
                float right_weight = (float)(mod)/(float)(skip);

                verts[3].position.y = left_height * left_weight +
                                      right_height * right_weight;
                verts[3+verts_per_quad*n_side-1].position.y = verts[3].position.y; // overlapping vertex in the next quad
            }
        } // for
    } // fix east edge

    if (lvldiff_nsew[3] < 0)
    {
        int skip = pow(2, std::abs(lvldiff_nsew[3])); // 2, 4, 8 etc

        int corner_offset = 1; // +first vertex of first column
        int skip_vert_stride = skip*(verts_per_quad*n_side);

        float left_height = vertices[corner_offset].position.y;
        float right_height = vertices[corner_offset + skip_vert_stride-verts_per_quad*n_side-1].position.y;

        int j = 0;
        for (int i = 0; i<n_side-1; i++) // all rows
        {
            int first_vert_index = verts_per_quad*(i*n_side + j);
            Vertex* verts = &(vertices[first_vert_index]);

            int edge_num = i+1;
            int mod = edge_num%skip;

            if (mod==0)
            {
                //update interpolants
                left_height = verts[0].position.y;
                right_height = verts[0 + skip_vert_stride].position.y;
            }
            else
            {
                float left_weight = 1.0-(float)(mod)/(float)(skip);
                float right_weight = (float)(mod)/(float)(skip);

                verts[0].position.y = left_height * left_weight +
                                      right_height * right_weight;
                verts[0+verts_per_quad*n_side+1].position.y = verts[0].position.y; // overlapping vertex in the next quad
            }
        } // for
    } // fix west edge

//    // correct interior normals
//    for (int i = 0; i<n_side-1; i++)
//    {
//        for (int j = 0; j<n_side-1; j++)
//        {
//            int nw = verts_per_quad*(i*n_side + j);
//            int ne = verts_per_quad*(i*n_side + j+1);
//            int sw = verts_per_quad*((i+1)*n_side + j);
//            int se = verts_per_quad*((i+1)*n_side + j+1);
//            //     1-------2------x
//            //     | _0___/| ____/|
//            //     |/   1  |/     |
//            //     0------(3)-----x
//            //     | _____/| ____/|
//            //     |/      |/     |
//            //     x-------x------x
//
//            glm::vec3 av_normal = 0.25f*(vertices[nw+3].normal +
//                                        vertices[ne+0].normal +
//                                        vertices[sw+2].normal +
//                                        vertices[se+1].normal);
//
//            vertices[nw+3].normal = av_normal;
//            vertices[ne+0].normal = av_normal;
//            vertices[sw+2].normal = av_normal;
//            vertices[se+1].normal = av_normal;
//        }
//    }

    // finished
}

//int Terrain::lin_i(int i, int j)
//{
//    return i*map_dim+j;
//};

void Terrain::generateGraphicsPatch(glm::vec3 center, float patch_length, unsigned int num_subd)
{
    Vertex* verts;
    Triangle* tris;
    int num_verts;
    int num_tris;

//            std::cout << "num_verts: " << num_verts << "\n";
//            std::cout << "num_tris: " << num_tris << "\n";
    int lvldiff_nsew[] = {0, 0, 0, 0};

    subdividedQuads(verts, num_verts, tris, num_tris, patch_length/2, num_subd, lvldiff_nsew, center.x, center.z);




     // assing the quad to the mesh;
    terrain_patches.push_back(TerrainPatch(num_subd)); // generates both new mesh and prop
    TerrainPatch* terrain_patch = &(terrain_patches.back());

    std::shared_ptr<Mesh> terrain_quad = terrain_patch->mesh;
    terrain_quad->fromMemory(verts, num_verts, tris, num_tris);

    delete [] verts;
    delete [] tris;


    terrain_patch->prop->pos = glm::vec3(center.x, 0.0f, center.z);

    // assign an axis aligned bounding box (used for culling)
    terrain_patch->world_aabb = QuadAABB({center.x-patch_length/2.0f, center.x+patch_length/2.0f, center.z-patch_length/2.0f, center.z+patch_length/2.0f});

    // get the placeholder terrain texture
    //std::weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("simple_plane");
    std::weak_ptr<Mesh>      mesh_ptr    = std::weak_ptr<Mesh>(terrain_quad);
    std::weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("grass_equal");
    std::weak_ptr<Texture>   tex2_ptr     = global::tex_manager.getResptrFromKey  ("rock");

    // attach the generated geometry to the patch-prop
    auto render_batch = terrain_patch->prop->attachBatch(mesh_ptr, tex_ptr);
    render_batch->addSecondTex(tex2_ptr);
}


void Terrain::generatePhysicsPatch(glm::vec3 center, float sideLength, int num_quads)
{
    m_dimPhysHeights = num_quads + 1;
    m_sidePhysHeights = sideLength;

    if (!m_heightData0) m_heightData0 = new float [m_dimPhysHeights*m_dimPhysHeights];

    float spacing = sideLength/((float)(num_quads));

    m_spacingPhysHeights = spacing;

    float min_height = 99999.0;
    float max_height = -99999.0;

    for (unsigned int i = 0; i<m_dimPhysHeights; i++)
    {
       for (unsigned int j = 0; j<m_dimPhysHeights; j++)
       {
            float x = center.x-sideLength/2.0 + j*spacing;
            float z = center.z-sideLength/2.0 + i*spacing;

            int linear_index = i*m_dimPhysHeights + j;

            m_heightData0[linear_index] = m_heightScale*sampleHeightMap(x, z, m_horzScale, m_centerX, m_centerZ);

            if (m_heightData0[linear_index] < min_height)
                min_height = m_heightData0[linear_index];

            if (m_heightData0[linear_index] > max_height)
                max_height = m_heightData0[linear_index];


            if (i<3 && j<3)
            {
                //std::cout << "height_corner " << m_heightData0[i*dim + j] << "\n";
                std::cout << "height "<< x <<", " << z << ": "<< m_heightData0[i*m_dimPhysHeights + j] << "\n";
            }

       }
    }

    m_terrainBody = m_physicsWorld->addStaticTerrainPatch(m_heightData0, m_dimPhysHeights, spacing, min_height, max_height, center);
}

void Terrain::updatePhysicsTerrain(glm::vec3 center)
{
    float min_height = 99999.0;
    float max_height = -99999.0;

    for (unsigned int i = 0; i<m_dimPhysHeights; i++)
    {
       for (unsigned int j = 0; j<m_dimPhysHeights; j++)
       {
            float x = center.x-m_sidePhysHeights/2.0 + j*m_spacingPhysHeights;
            float z = center.z-m_sidePhysHeights/2.0 + i*m_spacingPhysHeights;

            int linear_index = i*m_dimPhysHeights + j;

            m_heightData0[linear_index] = m_heightScale*sampleHeightMap(x, z, m_horzScale, m_centerX, m_centerZ);

            if (m_heightData0[linear_index] < min_height)
                min_height = m_heightData0[linear_index];

            if (m_heightData0[linear_index] > max_height)
                max_height = m_heightData0[linear_index];

       }
    }

//    m_physicsWorld->updateStaticTerrainPatch(m_terrainBody, min_height, max_height, center);
}

double rand_range(double a, double b)
{
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

void Terrain::generateHeightMap()
{ //UNINITCOND azb stack allocated, check local variables: PASS 1: Trivial -->
    double range = 8000.0; // m

    //srand(23798); // removing this gives a nice one
    //srand(12345); // removing this gives a nice one

    // find the first (middle point) m_dimension/2
    m_height_map(m_dimension/2, m_dimension/2) = rand_range(-range, range);

//        std::cout << "first point: " << m_height_map(m_dimension/2, m_dimension/2) << "\n";

    // find the first corner
    m_height_map(0,0) = m_height_map(m_dimension/2, m_dimension/2) + rand_range(-range/2.0, range/2.0);

//        std::cout << "corner point: " << m_height_map(m_dimension, m_dimension) << "\n";
//        std::cout << "corner point: " << m_height_map(0, 0) << "\n";

    // find the first edge points
    m_height_map(m_dimension/2, 0) = m_height_map(0, 0) + rand_range(-range/2.0, range/2.0);
    m_height_map(0, m_dimension/2) = m_height_map(0, 0) + rand_range(-range/2.0, range/2.0);

//        std::cout << "edge point: " << m_height_map(m_dimension, m_dimension) << "\n";

    unsigned int max_size = m_dimension+1;
    unsigned int depth = 0;
    unsigned int length_scale = (max_size-1)/pow(2, depth);

    while (length_scale > 2) // each layer
    {
//            std::cout << "level===================================\n";

        length_scale = (max_size-1)/pow(2, depth);
//            std::cout << "length_scale = " << length_scale << "\n";
        unsigned int num_quads = (max_size-1)/length_scale;
//            std::cout << "num_quads = " << num_quads << "\n";

        int scale_factor = pow(2, depth);

        for (unsigned int j = 0; j<num_quads; j++)
        {
//                std::cout << "yrow========================\n";
            // update y
            unsigned int low_y = j*length_scale;
            unsigned int high_y = (j+1)*length_scale;
            for (unsigned int i = 0; i<num_quads; i++)
            {
//                    std::cout << "xrow============\n";
                unsigned int low_x = i*length_scale;
                unsigned int high_x = (i+1)*length_scale;

                // fill in edges
//                    if (length_scale==2 && low_x == 32)
//                    {
//                        std::cout << "low_x = " << low_x << "\n";
//                        std::cout << "high_x = " << high_x << "\n";
//                        std::cout << "low_y = " << low_y << "\n";
//                        std::cout << "high_y = " << high_y << "\n";
//                    }


                m_height_map((low_x+high_x)/2, low_y) = (m_height_map(low_x, low_y)+m_height_map(high_x, low_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                m_height_map((low_x+high_x)/2, high_y) = (m_height_map(low_x, high_y)+m_height_map(high_x, high_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                m_height_map(low_x, (low_y+high_y)/2) = (m_height_map(low_x, low_y)+m_height_map(low_x, high_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);
                m_height_map(high_x, (low_y+high_y)/2) = (m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/2.0 +
                                             rand_range(-range/2.0/scale_factor, 0.0+range/2.0/scale_factor);

                // middle
//                    m_height_map((low_x+high_x)/2, (low_y+high_y)/2) = (m_height_map(low_x, low_y)+m_height_map(low_x, high_y)+m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/4.0 +
//                                                 rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);

                // Need to change this to make smoother middle (take all 9 surrounding into account with different weights)
                m_height_map((low_x+high_x)/2, (low_y+high_y)/2) = 0.414213562*(m_height_map(low_x, low_y)+m_height_map(low_x, high_y)+m_height_map(high_x, low_y)+m_height_map(high_x, high_y))/4.0
                                             + 0.585786438*(m_height_map((low_x+high_x)/2, low_y) + m_height_map((low_x+high_x)/2, high_y) + m_height_map(low_x, (low_y+high_y)/2) + m_height_map(high_x, (low_y+high_y)/2))/4.0
                                             + rand_range(-range/2.0/scale_factor/2.0, 0.0+range/2.0/scale_factor/2.0);
            }

        }
    depth++;
    }

    // adjust water lvl
    float av_height = 0.f; // oh fudge... this one was causing UNINITCOND before
    float num_cells = m_dimension*m_dimension;

    for (int i = 0; i<m_dimension; i++)
    {
        for (int j = 0; j<m_dimension; j++)
        {
            av_height+=m_height_map(i,j)/num_cells;
        }
    }

    for (int i = 0; i<m_dimension; i++)
    {
        for (int j = 0; j<m_dimension; j++)
        {
            m_height_map(i,j)=m_height_map(i,j)-av_height;
        }
    }


    // debug height
    unsigned int x_side = 79 < m_dimension? 79 : m_dimension;
    unsigned int y_side = 46 < m_dimension? 46 : m_dimension;

//        std::cout << "seed = " << m_seed << ":\n";
    for (unsigned int j = 0; j<y_side; j++)
    {
        for (unsigned int i = 0; i<x_side; i++)
        {
            unsigned int x_ind = int((float)(i)/(float)(x_side)*m_dimension);
            unsigned int y_ind = int((float)(j)/(float)(y_side)*m_dimension);

            float height = m_height_map(y_ind, x_ind);
            if (height < 0.0)
            {
                std::cout << "-";
//                std::printf("\x1b[34m"     "-"     "\x1b[0m" "\n");
            }
            else if (height < 500.0)
            {
                std::cout << "=";
            }
            else if (height < 1500.0)
            {
                std::cout << "o";
            }
            else if (height < 3000.0)
            {
                std::cout << "U";
            }
            else
            {
                std::cout << "A";
            }
        }
        std::cout << "\n";
    } // finished printing debug map
}

float Terrain::sampleHeightMap(float x, float z, float length, float center_x, float center_z) const
{
    // find the x and z of the top left corner
    float top_left_x = center_x-length/2.0;
    float top_left_z = center_z-length/2.0;

    float rel_x = x-top_left_x;
    float rel_z = z-top_left_z;

    float frac_x = rel_x/length;
    float frac_z = rel_z/length;

    float dec_ind_x = frac_x*m_dimension;
    float dec_ind_z = frac_z*m_dimension;

    int upper_x = ceil(dec_ind_x);
    int upper_z = ceil(dec_ind_z);
    int lower_x = floor(dec_ind_x);
    int lower_z = floor(dec_ind_z);

    if (upper_x == lower_x) upper_x++;
    if (upper_z == lower_z) upper_z++;

    float wt11 = (upper_x-dec_ind_x)*(upper_z-dec_ind_z)/((upper_x-lower_x)*(upper_z-lower_z));
    float wt12 = (dec_ind_x-lower_x)*(upper_z-dec_ind_z)/((upper_x-lower_x)*(upper_z-lower_z));
    float wt21 = (upper_x-dec_ind_x)*(dec_ind_z-lower_z)/((upper_x-lower_x)*(upper_z-lower_z));
    float wt22 = (dec_ind_x-lower_x)*(dec_ind_z-lower_z)/((upper_x-lower_x)*(upper_z-lower_z));

//    std::cout << "======================================================\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!d " << dec_ind_x << "\n";removePhysicsObject
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!u " << upper_x << "\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << lower_x << "\n";
//
//    std::couft << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!d " << wt11 << "\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!u " << wt12 << "\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << wt21 << "\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << wt22 << "\n";
//
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!d " << m_height_map(lower_x, lower_z) << "\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!u " << m_height_map(lower_x, upper_z) << "\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << m_height_map(upper_x, lower_z) << "\n";
//    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!l " << m_height_map(upper_x, upper_z) << "\n";
    float out = wt11*m_height_map(lower_x, lower_z) +
                wt12*m_height_map(lower_x, upper_z) +
                wt21*m_height_map(upper_x, lower_z) +
                wt22*m_height_map(upper_x, upper_z);

    return out;
}

void Terrain::updateObserverPosition(glm::vec3 observer_position)
{


    if (phys_anchor != anchor_position) // swap physics the frame after swapping graphics
    {
        m_physicsWorld->removeBtRigidBody(m_terrainBody);

        std::cout << "swapped physics\n";

        phys_anchor = anchor_position; // this snaps the physics away from graphics

        generatePhysicsPatch(phys_anchor, 2*m_patchLength, 2*pow(2, m_numPatchSubd));
    }
//    float dist_to_phys_anchor = pow(observer_position.x-phys_anchor.x, 2) +
//                                pow(observer_position.z-phys_anchor.z, 2);
//    if (dist_to_phys_anchor > phys_anchor_l_sq)
//    {
//
//    }

    // check the difference in x and z between observer and anchor
    float diff_x = observer_position.x-anchor_position.x;
    float diff_z = observer_position.z-anchor_position.z;

    float abs_diff_x = std::abs(diff_x);
    float abs_diff_z = std::abs(diff_z);

    float sign_x = (diff_x > 0.0) ? 1.0 : -1.0;
    float sign_z = (diff_z > 0.0) ? 1.0 : -1.0;

    //float start_offset = (float)(m_corePatchDim-1)/2.0;

    // if the difference is greater than threshold then move the anchor
    if (abs_diff_x > m_patchLength*0.6)
    {
        std::cout << "old anchor position: " << anchor_position.x << ", " << anchor_position.y << ", " << anchor_position.z << "\n";
        // do a swap in the x direction
        anchor_position = anchor_position + glm::vec3(sign_x*m_patchLength, 0.0, 0.0);

        std::cout << "new anchor position: " << anchor_position.x << ", " << anchor_position.y << ", " << anchor_position.z << "\n";

        std::cout << "length before: " << terrain_patches.size() << "\n";

        float erase_distance = ((float)(m_corePatchDim-1)/2.0)*1.01;

        // 2:

        for (auto it = terrain_patches.begin(); it<terrain_patches.end(); /*don't incr*/)
        {
            if (std::abs(it->prop->pos.x-anchor_position.x) > m_patchLength*erase_distance)
            {
                it = terrain_patches.erase(it);
            }
            else
            {
                it++;
            }

        }

        // create
        for (unsigned int i = 0; i<m_corePatchDim; i++)
        {

            float start_offset = (float)(m_corePatchDim-1)/2.0;

            float center_x = anchor_position.x + start_offset*sign_x*m_patchLength;
            float center_z = anchor_position.z + (i - start_offset)*m_patchLength;

            generateGraphicsPatch(glm::vec3(center_x, 0.0, center_z), m_patchLength, 0);
        }

        std::cout << "swapped x\n";

    }

    // if the difference is greater than threshold then move the anchor
    if (abs_diff_z > m_patchLength*0.80)
    {
        std::cout << "old anchor position: " << anchor_position.x << ", " << anchor_position.y << ", " << anchor_position.z << "\n";
        // do a swap in the z direction
        anchor_position = anchor_position + glm::vec3(0.0, 0.0, sign_z*m_patchLength);

        std::cout << "new anchor position: " << anchor_position.x << ", " << anchor_position.y << ", " << anchor_position.z << "\n";

        std::cout << "length before: " << terrain_patches.size() << "\n";

        float erase_distance = ((float)(m_corePatchDim-1)/2.0)*1.01;


        for (auto it = terrain_patches.begin(); it<terrain_patches.end(); /*don't incr*/)
        {
            if (std::abs(it->prop->pos.z-anchor_position.z) > m_patchLength*erase_distance)
            {
                it = terrain_patches.erase(it);
            }
            else
            {
                it++;
            }

        }

        // create
        for (unsigned int j = 0; j<m_corePatchDim; j++)
        {
            float start_offset = (float)(m_corePatchDim-1)/2.0;

            float center_x = anchor_position.x + (j - start_offset)*m_patchLength;;
            float center_z = anchor_position.z + start_offset*sign_z*m_patchLength;

            generateGraphicsPatch(glm::vec3(center_x, 0.0, center_z), m_patchLength, 0);
        }

        std::cout << "swapped z\n";

        // Move the physics as well (just lazy, could add another
        // anchor to de-stress the CPU load of the "swapping frame"
    }

    //std::cout << ">>>>>>>>>>>going through patches\n";
    // go through all patches, if they are wrong level change them

    //bool changed_terrain = false;
    for (auto it = terrain_patches.begin(); it<terrain_patches.end(); it++)
    {
        float del_x = it->prop->pos.x - observer_position.x;
        float del_z = it->prop->pos.z - observer_position.z;
        float sq_distance = del_x*del_x + del_z*del_z;
        //std::cout << max_dist << "\n";

        int pref_lvl = getPrefLvl(sq_distance);

        //std::cout << "  dist_sq = " << sq_distance << ", lvl should be " << pref_lvl << ", is " << it->subd_lvl << "\n";

        // check difference in lvl of surroudning patches
        // if lower, then account for this in subdivision
        int preflvldiff_nsew[] = {0, 0, 0, 0};

        { // north
            float ndel_z = it->prop->pos.z - m_patchLength - observer_position.z;
            float nsq_distance = del_x*del_x + ndel_z*ndel_z;

            preflvldiff_nsew[0] = getPrefLvl(nsq_distance) - pref_lvl;
        }

        { // south
            float sdel_z = it->prop->pos.z + m_patchLength - observer_position.z;
            float ssq_distance = del_x*del_x + sdel_z*sdel_z;

            preflvldiff_nsew[1] = getPrefLvl(ssq_distance) - pref_lvl;
        }

        { // east
            float edel_x = it->prop->pos.x + m_patchLength - observer_position.x;
            float esq_distance = edel_x*edel_x + del_z*del_z;

            preflvldiff_nsew[2] = getPrefLvl(esq_distance) - pref_lvl;
        }

        { // west
            float wdel_x = it->prop->pos.x - m_patchLength - observer_position.x;
            float wsq_distance = wdel_x*wdel_x + del_z*del_z;

            preflvldiff_nsew[3] = getPrefLvl(wsq_distance) - pref_lvl;
        }

//        std::cout << "level diff: ";
//        for (int i = 0; i<4; i++)
//        {
//            std::cout << preflvldiff_nsew[i] << ", ";
//        }
//        std::cout << "\n";
        //std::cout << ">" << it->prop->pos.x/m_patchLength << ", " << it->prop->pos.z/m_patchLength << " ";
        //std::cout << "own: " << pref_lvl << ", nsew: ";
        int sum_deviances = std::abs(pref_lvl-it->subd_lvl);
        for (int i = 0; i<4; i++)
        {
            sum_deviances += std::abs(preflvldiff_nsew[i]-it->diff_lvl[i]);
           // std::cout << preflvldiff_nsew[i] << " ";
        }
        //std::cout << "\n";






        if (sum_deviances > 0)
        {
            //changed_terrain = true;
            changeSubdLvl(pref_lvl, &(*it), preflvldiff_nsew);
            //std::cout << "remedying\n";
        }
    }
//    if (changed_terrain==false)
//    {
//        std::cout << "no terrain changes\n";
//    }
    //std::cout << "updating terrain!\n";

}

int Terrain::getPrefLvl(float square_distance)
{
    // quadratic
    //return std::max((int)(ceil(m_numPatchSubd*(1-square_distance/m_maxDistSq))), 0);

    // linear
    if (square_distance < m_patchLength*m_patchLength)
    {
        return m_numPatchSubd;
    }
    else
    {
        return std::max((int)(ceil(m_numPatchSubd*(1-sqrt(square_distance/m_maxDistSq)))), 0);
    }

}

float Terrain::ySample(float x, float z) const
{
    return m_heightScale*sampleHeightMap(x, z, m_horzScale, m_centerX, m_centerZ);
}

glm::vec3 Terrain::normSample(float x, float z)
{
    float dx = 2.0;
    float dz = dx;

    float y_back_x = ySample(x-dx, z);
    float y_forw_x = ySample(x+dx, z);

    glm::vec3 del_x = glm::vec3 (2*dx, y_forw_x - y_back_x, 0.0);

    float y_back_z = ySample(x, z-dz);
    float y_forw_z = ySample(x, z+dz);

    glm::vec3 del_z = glm::vec3 (0.0, y_forw_z - y_back_z, 2*dz);

    glm::vec3 normal = glm::normalize(glm::cross(del_z, del_x));

    return normal;
}

void Terrain::changeSubdLvl(unsigned int subd_lvl_in, TerrainPatch* terrain_patch, int *lvldiff_nsew)
{
    glm::vec3 old_pos = terrain_patch->prop->pos;

    //std::cout << "Do any meshes get deleted here?\n"; //YES!!
    //delete what is already there? Check how magic the shared pointer is
    terrain_patch->reInit(subd_lvl_in);

    for (int i = 0; i<4; i++)
        terrain_patch->diff_lvl[i] = lvldiff_nsew[i];

    Vertex* verts;
    Triangle* tris;
    int num_verts;
    int num_tris;

//            std::cout << "num_verts: " << num_verts << "\n";
//            std::cout << "num_tris: " << num_tris << "\n";

    Terrain::subdividedQuads(verts,
                             num_verts,
                             tris,
                             num_tris,
                             m_patchLength/2.0,
                             subd_lvl_in,
                             lvldiff_nsew,
                             old_pos.x,
                             old_pos.z);

    std::shared_ptr<Mesh> terrain_quad = terrain_patch->mesh;
    terrain_quad->fromMemory(verts, num_verts, tris, num_tris);

    delete [] verts;
    delete [] tris;

    terrain_patch->prop->pos = old_pos;

    // assign an axis aligned bounding box (used for culling)
    terrain_patch->world_aabb = QuadAABB({old_pos.x-m_patchLength/2.0f, old_pos.x+m_patchLength/2.0f, old_pos.z-m_patchLength/2.0f, old_pos.z+m_patchLength/2.0f});


    // get the placeholder terrain texture
    //std::weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("simple_plane");
    std::weak_ptr<Mesh>      mesh_ptr    = std::weak_ptr<Mesh>(terrain_quad);
    std::weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("growth");
    std::weak_ptr<Texture>   tex2_ptr     = global::tex_manager.getResptrFromKey  ("rock");

    // attach the generated geometry to the patch-prop
    auto render_batch = terrain_patch->prop->attachBatch(mesh_ptr, tex_ptr);
    render_batch->addSecondTex(tex2_ptr);
    //std::cout << "After change then the new subd_lvl is" << terrain_patch->subd_lvl <<"\n";
}
