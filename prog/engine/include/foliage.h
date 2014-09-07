#ifndef FOLIAGE_H
#define FOLIAGE_H

#include "smallvisual.h"
#include "global.h"
#include "terrain.h"
#include "threadingwrapper.h"

class Foliage
{
    public:
        Foliage(Terrain* in_terrain);
        virtual ~Foliage();

        std::vector<SmallVisual> const * const getVisuals() const;

        void addSmallVisuals(string mesh_key,
                             string tex_key,
                             glm::vec4 wind_params,
                             glm::vec3 center,
                             float radius,
                             float density);

        struct BG_Thread
        {
            SmallVisual spruce;

            void prepareBG_Foliage()
            {
                spruce.init();

                spruce.mesh = std::shared_ptr<Mesh>(global::mesh_manager.getResptrFromKey("spruce_wbranch"));
                spruce.tex = std::shared_ptr<Texture>(global::tex_manager.getResptrFromKey("spruce_wbranch"));
                spruce.wind_params = glm::vec4(180.f, 20.f, 2.f, 0.0);
            };

        } bg_thread;

    protected:
    private:
        std::vector<SmallVisual> small_visuals;



        Terrain* terrain;
};

#endif // FOLIAGE_H
