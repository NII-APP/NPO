#include "identifyclassic.h"
#include "cmatrix.h"
#include "cgl.h"

IdentifyClassic::IdentifyClassic( CGL::CMatrix const & matrix ){

}

int CGL::maxIndex() const {
   T max = -100.;
   int maxN = 0;
   for( int i(0); i < height(); i++){
       if( m[i][1] > max ){
                max = m[i][1];
                maxN = i;
        }
    }
    return maxN;
}
