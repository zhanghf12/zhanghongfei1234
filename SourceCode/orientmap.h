#ifndef _ORIENTMAP_H
#define _ORIENTMAP_H
#include "densitymap.h"
#include "noise.h"
#include "g_cap.h"
#include "fstream"
namespace zhanghongfei
{
    class OrientMap : public DensityMaps, public G_Cap
    {
    public:
        OrientMap();
        ~OrientMap();
        float get_orient(int i, int j, int singularity_type);
        void orientmap(void);
        //void draw_orient(float** orient_padded_few);

    public:
        float arch_fact1;
        float arch_fact2;
        float** orient;
        int singularity_type;
    };
}
#endif
