///// PROPOSED NEW IMPLEMENTATION
//

#include "skeleton.h"

///---------------------------------------------------

Skeleton::Skeleton() : num_bones(0),
                       bones(nullptr),
                       num_anims(0),
                       animations(nullptr),
                       triggered(false)
{
    //ctor
}

Skeleton::~Skeleton()
{
    delete [] bones;
    delete [] animations;
}

void Skeleton::fromFile(std::string skel_key)
{
    bool debug = false;

    std::string filepath = "assets/skeletons/"+skel_key+".bbns";

    std::streampos filesize;
    char * memblock;

    std::ifstream myFile (filepath, std::ios::in | std::ios::binary | std::ios::ate);

    if (myFile.is_open())
    {
        filesize = myFile.tellg();
        memblock = new char [filesize];
        myFile.seekg (0, std::ios::beg);
        myFile.read (memblock, filesize);
        myFile.close();

        if (debug) std::cout << filesize << "\n";

        std::string filetype;
        filetype.resize(4*sizeof(char));      /// 4 bytes     file type
        int verMaj;                           /// 4 bytes     version major
        int verMin;                           /// 4 bytes     version minor

        MemStreamer reader(memblock, 0, filesize);
        reader.chomp(&filetype[0],  4*sizeof(char));
        reader.chomp(&verMaj,       1*sizeof(int));
        reader.chomp(&verMin,       1*sizeof(int));


        if (debug) std::cout << filetype << "\n";
        if (debug) std::cout << verMaj << "\n";
        if (debug) std::cout << verMin << "\n";

        /// READ NUMBER OF BONES
        reader.chomp(&num_bones,       1*sizeof(int));
        if (debug) std::cout << "num of bones: " << num_bones << "\n";

        // init bones
        bones = new Bone [num_bones];

        for (int u = 0; u<num_bones; u++)
        {
            int bone_index_u;
            reader.chomp(&bone_index_u,       1*sizeof(int));
            if (debug) std::cout << "bone index: "<< bone_index_u << "\n";

            Bone* bone = &bones[u];

            /// Write the name
            int nameLen;
            reader.chomp(&nameLen, 1*sizeof(int));

            bone->name.resize(nameLen*sizeof(char));      /// 4 bytes     file type
            reader.chomp(&bone->name[0],       nameLen*sizeof(char));
            if (debug) std::cout << "name: " << bone->name << "\n";

            /// READ THE BONE MATRIX
            float matarr[16];
            reader.chomp(&matarr[0],       16*sizeof(float));
            for (int i = 0; i<4; i++)
            {
                for (int j = 0; j<4; j++)
                {
                    bone->rest_matrix[i][j] = matarr[j+4*i]; // Store row major
                    if (debug) std::cout << bone->rest_matrix[i][j] << "\t";
                }
                if (debug) std::cout << "\n";
            }

            reader.chomp(&bone->num_children, 1*sizeof(int));
            if (debug) std::cout << "num children: " << bone->num_children<<"\n";

            bone->child_indices = new int [bone->num_children];
            reader.chomp(bone->child_indices, bone->num_children*sizeof(int));
            if (debug) std::cout << "children: ";
            for (int i = 0; i<bone->num_children; i++) if (debug) std::cout << bone->child_indices[i] << "\t";
            if (debug) std::cout << "\n";
        } /// Finished loading bones

        /// Animations
        reader.chomp(&num_anims, 1*sizeof(int));
        if (debug) std::cout << "num anims: " << num_anims << "\n";

        animations = new Animation [num_anims];

        for (int u = 0; u<num_anims; u++)
        {
            int anim_index_u;
            reader.chomp(&anim_index_u,       1*sizeof(int));
            Animation* anim = &animations[anim_index_u];
            if (debug) std::cout << "anim index: " << anim_index_u << "\n";

            /// Duration
            reader.chomp(&anim->duration, 1*sizeof(float));
            if (debug) std::cout<<"duration: " << anim->duration << "\n";

            /// Number of channels
            reader.chomp(&anim->num_channels, 1*sizeof(int));
            anim->channels = new Animation::Channel [anim->num_channels];
            if (debug) std::cout << "num channels: " << anim->num_channels << "\n";

            for (int c = 0; c<anim->num_channels; c++)
            {
                int channel_index_c;
                reader.chomp(&channel_index_c,       1*sizeof(int));
                Animation::Channel* channel = &anim->channels[channel_index_c];
                if (debug) std::cout << "channel index: " << channel_index_c << "\n";
                channel->bone_index = channel_index_c;

                /// Read number of position keys
                reader.chomp(&channel->pos_series.num_keys,       1*sizeof(int));
                channel->pos_series.keys = new TimeValuePair<glm::vec3> [channel->pos_series.num_keys];
                channel->pos_series.duration = anim->duration;
                if (debug) std::cout << "num pos keys: " << channel->pos_series.num_keys << "\n";

                for (int k = 0; k<channel->pos_series.num_keys; k++)
                {
                    /// Time
                    TimeValuePair<glm::vec3>* timePosPair = &channel->pos_series.keys[k];
                    reader.chomp(&timePosPair->time, 1*sizeof(float));
                    if (debug) std::cout << "time: " << timePosPair->time << "\n";

                    /// Vector
                    reader.chomp(&(timePosPair->value[0]), 3*sizeof(float));
                    if (debug) std::cout << "pos: [" << timePosPair->value.x << "\t"
                                          << timePosPair->value.y << "\t"
                                          << timePosPair->value.z << "]\n";
                }

                /// Read number of rotation keys
                reader.chomp(&channel->rot_series.num_keys,       1*sizeof(int));
                channel->rot_series.keys = new TimeValuePair<glm::quat> [channel->rot_series.num_keys];
                channel->rot_series.duration = anim->duration;
                if (debug) std::cout << "num rot keys: " << channel->rot_series.num_keys << "\n";

                for (int k = 0; k<channel->rot_series.num_keys; k++)
                {
                    /// Time
                    TimeValuePair<glm::quat>* timeRotPair = &channel->rot_series.keys[k];
                    reader.chomp(&timeRotPair->time, 1*sizeof(float));
                    if (debug) std::cout << "time: " << timeRotPair->time << "\n";

                    /// Quaternion
                    reader.chomp(&(timeRotPair->value.w), 1*sizeof(float));
                    reader.chomp(&(timeRotPair->value.x), 1*sizeof(float));
                    reader.chomp(&(timeRotPair->value.y), 1*sizeof(float));
                    reader.chomp(&(timeRotPair->value.z), 1*sizeof(float));
                    if (debug) std::cout << "rot: [" << timeRotPair->value.w << "\t"
                                          << timeRotPair->value.x << "\t"
                                          << timeRotPair->value.y << "\t"
                                          << timeRotPair->value.z << "]\n";
                }

                /// Read number of scaling keys
                reader.chomp(&channel->sca_series.num_keys,       1*sizeof(int));
                channel->sca_series.keys = new TimeValuePair<glm::vec3> [channel->sca_series.num_keys];
                channel->sca_series.duration = anim->duration;
                if (debug) std::cout << "num sca keys: " << channel->sca_series.num_keys << "\n";

                for (int k = 0; k<channel->sca_series.num_keys; k++)
                {
                    /// Time
                    TimeValuePair<glm::vec3>* timeScaPair = &channel->sca_series.keys[k];
                    reader.chomp(&timeScaPair->time, 1*sizeof(float));
                    if (debug) std::cout << "time: " << timeScaPair->time << "\n";

                    /// Vector
                    reader.chomp(&(timeScaPair->value[0]), 3*sizeof(float));
                    if (debug) std::cout << "sca: [" << timeScaPair->value.x << "\t"
                                          << timeScaPair->value.y << "\t"
                                          << timeScaPair->value.z << "]\n";
                }
                /// Fill helper variable (used for seek)
                channel->ch_duration = anim->duration;
            }
        }

        delete[] memblock;
//        delete filetype;
    }
    else
    {
        std::cout << "failed to open file " << filepath << "\n";
    }
}

void Skeleton::poseMatrices(glm::mat4* matrices,
                            int anim_index,
                            float time,
                            int bone_index,
                            glm::mat4 parent_mat)
{
    bool debug = true;

    if (anim_index<num_anims && anim_index > -1 && num_bones > 0)
    {
        Animation* animation = &animations[anim_index];

        Bone* bone = &bones[bone_index];
        Animation::Channel* channel = &(animation->channels[bone_index]);

        /// Seek (find keyframes to interpolate and their weights
        auto pos_res = channel->pos_series.seek(time);
        auto rot_res = channel->rot_series.seek(time);
        auto sca_res = channel->sca_series.seek(time);

        /// Preform interpolation
        glm::vec3 key_pos = glm::mix(pos_res.prev.key->value,
                                     pos_res.next.key->value,
                                     pos_res.next.weight);

        glm::quat key_rot = helper::qslerp(rot_res.prev.key->value, // glm::mix is not sufficient in this case
                                          rot_res.next.key->value, // it will not choose the same path
                                          rot_res.next.weight); // will oscillate between equiv solutions quat = -quat

        glm::vec3 key_sca = glm::mix(sca_res.prev.key->value,
                                     sca_res.next.key->value,
                                     sca_res.next.weight);


        /// Construct transformation matrices
        glm::mat4 trans_mat = glm::translate(glm::mat4(1.0), key_pos);
        glm::mat4 rot_mat = glm::mat4_cast(key_rot);
        glm::mat4 sca_mat = glm::scale(glm::mat4(1.0), key_sca);

        /// Construct local transformation matrix
        glm::mat4 local_mat = trans_mat * rot_mat * sca_mat;

        /// Premultiply by parent to create object space matrix
        glm::mat4 pose_mat = parent_mat * local_mat;

        /// Add the matrix to the output (still don't understand why this must be transposed
        /// and not modelViewMatrix etc...)
        matrices[bone_index] = glm::transpose(pose_mat * glm::inverse(bones[bone_index].rest_matrix));


        /// Recursively pose child bone matrices
        for (int i_child=0; i_child<bone->num_children; i_child++)
        {
            poseMatrices(matrices,
                         anim_index,
                         time,
                         bone->child_indices[i_child],
                         pose_mat);
        }

    }
    else
    {
        std::cerr << "invalid skeleton pose call\n";
        std::cerr << "setting all bone matrices to rest\n";
        for (int i=0; i<num_bones; i++)
        {
            matrices[i] = bones[i].rest_matrix;
        }
    }
}


///---------------------------------------------------



Skeleton::Animation::Animation() : num_channels(0),
                                   channels(nullptr)
{
    //ctor
}

Skeleton::Animation::~Animation()
{
    delete [] channels;
}


///---------------------------------------------------


Skeleton::Animation::Channel::Channel()
{
    //ctor
}

Skeleton::Animation::Channel::~Channel()
{

}

///---------------------------------------------------


Skeleton::Pose::Pose() : num_transforms(0),
                         transforms(nullptr)
{
    // ctor
}

Skeleton::Pose::~Pose()
{
    delete [] transforms;
}

Skeleton::Pose::Pose(int num_transforms_in)
{
    allocate(num_transforms_in);
}

Skeleton::Pose::Pose(Skeleton* skel)
{
    allocate(skel->getNumBones());
}

void Skeleton::Pose::allocate(int num_transforms_in)
{
    num_transforms = num_transforms_in;
    transforms = new Transform [num_transforms];
}
