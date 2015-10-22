#include "identifyclassic.h"
#include "cmatrix.h"
#include "cgl.h"

IdentifyClassic::IdentifyClassic( const CGL::CMatrix & matrix ){

}
IdentifyClassic::~IdentifyClassic( const CGL::CMatrix & matrix ){

}
int IdentifyClassic::maxIndex(){
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
