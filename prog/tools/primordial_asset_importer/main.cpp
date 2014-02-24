#include <iostream>
#include <string>
#include <fstream>

#include "converter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int main()
{
    convert("test_scenes/mdl_human_male2.dae",
            "../../engine/assets_bin/models/human_male_bgeo.bgeo",
            "../../engine/assets_bin/skeletons/human_male_bgeo.bbns");

    convert("test_scenes/asset_anim_test6.dae",
            "../../engine/assets_bin/models/anim_test.bgeo",
            "../../engine/assets_bin/skeletons/anim_test.bbns");

    return 0;
}
