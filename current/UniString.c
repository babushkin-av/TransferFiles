

LPTSTR lstrcpyn(TCHAR *Dest, TCHAR *Src, int nCch){

    if( !Src )  Dest = NULL;
    if( Dest )
    {
        *TCHAR *DestMax = (Dest+nCch-1);

        do
        {   TCHAR temp = *Src;
            if( *Dest = temp )  break;

            Src++;
            if( (++Dest) < DestMax ) {   *Dest = 0;  break;   };

        }while(true);
    };
return(Dest); }
