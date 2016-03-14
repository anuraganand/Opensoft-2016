//Link libHaru lib ------       export LD_LIBRARY_PATH="/lib:/usr/lib:/usr/local/lib"
//Commands:
// g++ -c <filename.c> -o <filename.o>
//  gcc <filename.o> -L"/usr/local/lib" -lhpdf -o "filename"
// ./<filename>
//The ouput pdf will be saved in the same folder a <filename.c>
//Used random numbers for generating the no.of rows and columns. Can be modified accordingly.


#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"
#include <math.h>
#include "hpdf.h"

jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler (HPDF_STATUS   error_no,
               HPDF_STATUS   detail_no,
               void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}

void
insert_text (HPDF_Page page,float x,float y,const char  *label)
{
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y );
    HPDF_Page_ShowText (page, label);
    HPDF_Page_EndText (page);
}

void
draw_rect (HPDF_Page     page,
           double        x,
           double        y,
           const char   *label)
{
    HPDF_Page_BeginText (page);
    HPDF_Page_MoveTextPos (page, x, y - 10);
    HPDF_Page_ShowText (page, label);
    HPDF_Page_EndText (page);

    HPDF_Page_Rectangle(page, x, y - 40, 220, 25);
}
//void setwidth(HPDF_PDF pdf,int width)
//{
  //  HPDF_Page page = HPDF_GetCurrentPage (pdf);
    //HPDF_Page_SetWidth (page, HPDF_Image_GetWidth (image));
//}
int main (int argc, char **argv)
{
    HPDF_Doc  pdf;
    char fname[256];    
    HPDF_Font def_font,font;
    HPDF_REAL tw;
    HPDF_REAL height;
    HPDF_REAL width;
    HPDF_UINT i;
    //pages is the no.of tables required to be produced
    int pages;
    printf("Enter no.of tables: ");
    scanf("%d",&pages);
    //printf("No of pages : %d\n",pages);
    //the arrays rows and columns contain the corresponding row count and column count of that table
    //Eg:
    int rows[pages];
    int cols[pages];

    int it;
    srand(time(NULL));
    for(it=0;it<=pages;it++)
    {
        //srand(time(NULL));
        rows[it] = 2+rand()%30;
        //srand(time(NULL));
        cols[it] = 1+rand()%15;
    }
    strcpy (fname, argv[0]);
    strcat (fname, ".pdf");
    
    pdf = HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }
    int kl;
    for(kl=0;kl<=pages;kl++)
    {
        /* Add a new page object. */
        HPDF_Page page;
        page = HPDF_AddPage (pdf);
        const char *page_title = "Table";
        //char *ch = (char)(kl);
        //strcat(page_title,ch);
        HPDF_Page_SetHeight(page,rows[kl]*10 + 100);
        HPDF_Page_SetWidth (page,cols[kl]*75 + 200);
        height = HPDF_Page_GetHeight (page);
        width = HPDF_Page_GetWidth(page);
        /* Print the boxes of the page. */
        int il=1,jl;
        for(jl=1;jl<=cols[kl];jl++)
        {
            HPDF_Page_Rectangle (page, 10 + jl*75 , -30+height - (il+3)*10, 75, 10);   
        }
    
        for(il=2;il<=rows[kl];il++)
        {
            for(jl=1;jl<=cols[kl];jl++)
            {
                HPDF_Page_Rectangle (page, 10 + jl*75 , -30+height - (il+3)*10, 75, 10);   
            }
        }
    
        HPDF_Page_Stroke (page);

        /* Print the title of the page (with positioning center). */
        def_font = HPDF_GetFont (pdf, "Helvetica", NULL);
        HPDF_Page_SetFontAndSize (page, def_font, 24);

        tw = HPDF_Page_TextWidth (page, page_title);
        HPDF_Page_BeginText (page);
        HPDF_Page_TextOut (page, (width - tw) / 2, height - 50, page_title);
        HPDF_Page_EndText (page);

        HPDF_Page_SetFontAndSize (page, def_font, 10);

        /* Draw verious widths of lines. */
        il=1;
        HPDF_Page_SetLineWidth (page, 0);
        for(jl=1;jl<=cols[kl];jl++)
        {
            insert_text (page, 10 + jl*75 , -28+height - (il+3)*10, "Title");  
        }
        
        //Text in the boxes
        for(il=2;il <= rows[kl];il++)
        {
            for(jl=1;jl<=cols[kl];jl++)
            {
                HPDF_Page_SetLineWidth (page, 0);
                insert_text(page, 10 + jl*75, -29+height - (il+3)*10, "----" );
            }
            
        }

        /* output subtitle. */
        HPDF_Page_BeginText (page);
        HPDF_Page_SetFontAndSize (page, def_font, 16);
        //HPDF_Page_TextOut (page, 60, height - 80, "<Standerd Type1 fonts samples>");
        HPDF_Page_EndText (page);

        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page, 60, height - 105);

        HPDF_Page_EndText (page);

    }

    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    return 0;
}
