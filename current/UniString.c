
/**************************************************************************************************************************
 * =============================================== *** Header files *** ================================================= *
 **************************************************************************************************************************/

#include "UniString.h"

/**************************************************************************************************************************
 * =============================================== * lstrcpy() function * =============================================== *
 **************************************************************************************************************************/

TCHAR* lstrcpy(TCHAR *Dest, CONST TCHAR *Src){

    if( !Src )  Dest = NULL;
    if( Dest )
        for(; *Dest=*Src; Src++,Dest++);

return(Dest); }

/**************************************************************************************************************************
 * =============================================== * lstrcpyn() function * ============================================== *
 **************************************************************************************************************************/

TCHAR* lstrcpyn(TCHAR *Dest, CONST TCHAR *Src, int nCch){

    if( !Src )  Dest = NULL;
    if( Dest )
        for(TCHAR *DestMax = (Dest+nCch); *Dest=*Src; Src++,Dest++)
            if( Dest < DestMax ) {   *Dest = 0;  break;   };

return(Dest); }

/**************************************************************************************************************************
 * ================================================ * lstrlen() function * ============================================== *
 **************************************************************************************************************************/

INT lstrlen(CONST TCHAR *Src){

    TCHAR *SrcMax = Src;

    if( SrcMax )
        while( *SrcMax ) SrcMax++;

return((INT)(TCHAR)(SrcMax-Src)); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

