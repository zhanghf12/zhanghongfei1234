#ifndef _DENSITYMAP_H
#define _DENSITYMAP_H

#include"global.h"
#include"noise.h"
#include"ahaque_complex.h"
namespace zhanghongfei {
	class DensityMaps : public Noise {
        public:
            DensityMaps();
            ~DensityMaps();
            void sel_n_merg_densitymap(void);
            void random_densitymap(void);
            void densitymap(void);
        public:
            int** f_den_int_2Dmat;
            float** f_den_2Dmat;
            complex l[3];
            complex d[3];
            int n_fp, n_fo;
	};
}

#endif
