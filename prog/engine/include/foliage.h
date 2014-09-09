#ifndef FOLIAGE_H
#define FOLIAGE_H

#include "smallvisual.h"
#include "global.h"
#include "terrain.h"
#include "threadingwrapper.h"
#include "quadtree.hpp"
#include "camera.h"

struct FolSpec
{
    public:
        enum class Type {Tree, Grass};
        FolSpec(glm::vec4 pos_in, Type type_in) :
            pos(pos_in), type(type_in),
            x(pos.x), z(pos.z)
             {};
        glm::vec4 pos;
        Type type;
        float x;
        float z;
    private:
        FolSpec();
};

class Foliage
{
    public:
        Foliage(Terrain* in_terrain);
        virtual ~Foliage();

        //std::vector<SmallVisual> const * const getVisuals() const;

        //void addSmallVisuals(string mesh_key,
//                             string tex_key,
//                             glm::vec4 wind_params,
//                             glm::vec3 center,
//                             float radius,
//                             float density);

        struct BG_Thread
        {
            BG_Thread() : qt_trees(QuadAABB({-500, 500, -500, 500})) {};

            SmallVisual spruce;
            SmallVisual grass;

            void prepareBG_Foliage()
            {
                spruce.init();

                spruce.mesh = std::shared_ptr<Mesh>(global::mesh_manager.getResptrFromKey("spruce_wbranch"));
                spruce.tex = std::shared_ptr<Texture>(global::tex_manager.getResptrFromKey("spruce_wbranch"));
                spruce.wind_params = glm::vec4(180.f, 20.f, 2.f, 0.0);

                grass.init();

                grass.mesh = std::shared_ptr<Mesh>(global::mesh_manager.getResptrFromKey("grass_spring"));
                grass.tex = std::shared_ptr<Texture>(global::tex_manager.getResptrFromKey("grass_spring"));
                grass.wind_params = glm::vec4(10.f, 10.f, 2.f, 0.0);
            };

            void updateFoliage(Camera &cam)
            {
                LockGuard lock_qt_trees(sm_mutex);

                int i_tree = 0;
                int i_grass = 0;

                spruce.updated = true;  // assume that the spruce on the gfx card is up to date
                grass.updated = true;

                qt_trees.for_all_in(cam.get2dViewFrustum(1.15, 0.5),
                //qt_trees.for_all_in(QuadAABB({-100, 100, -100, 100}),
                                    [&] (FolSpec &fol_dat)
                                    {
                                        switch (fol_dat.type)
                                        {
                                            case(FolSpec::Type::Tree):
                                            {
                                                if (i_tree<SmallVisual::MAX_NUM_SMVIS)
                                                {
                                                    if (spruce.updated && spruce.sm_buffer[i_tree]!=fol_dat.pos)
                                                    spruce.updated = false;

                                                    spruce.sm_buffer[i_tree] = fol_dat.pos;
                                                    i_tree++;
                                                } // if (i<SmallVisual::MAX_NUM_SMVIS)
                                            } break;
                                            case(FolSpec::Type::Grass):
                                            {
                                                if (i_grass<SmallVisual::MAX_NUM_SMVIS)
                                                {
                                                    if (grass.updated && spruce.sm_buffer[i_grass]!=fol_dat.pos)
                                                    grass.updated = false;

                                                    grass.sm_buffer[i_grass] = fol_dat.pos;
                                                    i_grass++;
                                                } // if (i<SmallVisual::MAX_NUM_SMVIS)
                                            } break;
                                        } // switch
                                    });
                spruce.num_smvis = i_tree;
                grass.num_smvis = i_grass;
            }

            Mutex sm_mutex;
            QuadTree<FolSpec, 5> qt_trees;

        } bg_thread;

    protected:
    private:
        //std::vector<SmallVisual> small_visuals;



        Terrain* terrain;
};

#endif // FOLIAGE_H
