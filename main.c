#include <stdio.h>
#include <stdlib.h>
#include<math.h>
typedef struct
{
    int a,b;
    double c;
    int d;
} coordonate;

int latime_imagine(FILE *f)
{
    int x=0;
    f= fopen("peppers.bmp" , "rb");
    fseek(f, 18, SEEK_SET);
    fread(&x , 1 , 2 , f);
    return x;
    fclose(f);
}
int inaltime_imagine(FILE *f)
{
    int x=0;
    f= fopen("peppers.bmp" , "rb");
    fseek(f, 22, SEEK_SET);
    fread(&x , 1 , 2 , f);
    return x;
    fclose(f);
}

void grayscale_image(char* nume_fisier_sursa,char* nume_fisier_destinatie)
{
    FILE *fin, *fout;
    unsigned int dim_img, latime_img, inaltime_img;
    unsigned char pRGB[3], aux;
    printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);
    fin = fopen(nume_fisier_sursa, "rb");
    if(fin == NULL)
    {
        printf("nu am gasit imaginea sursa din care citesc");
        return;
    }
    fout = fopen(nume_fisier_destinatie, "wb+");
    fseek(fin, 2, SEEK_SET);
    fread(&dim_img, sizeof(unsigned int), 1, fin);
    printf("Dimensiunea imaginii in octeti: %u\n", dim_img);
    fseek(fin, 18, SEEK_SET);
    fread(&latime_img, sizeof(unsigned int), 1, fin);
    fread(&inaltime_img, sizeof(unsigned int), 1, fin);
    printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n",latime_img, inaltime_img);
    fseek(fin,0,SEEK_SET);
    unsigned char c;
    while(fread(&c,1,1,fin)==1)
    {
        fwrite(&c,1,1,fout);
        fflush(fout);
    }
    fclose(fin);
    int padding;
    if(latime_img % 4 != 0)
        padding = 4 - (3 * latime_img) % 4;
    else
        padding = 0;
    printf("padding = %d \n",padding);
    fseek(fout, 54, SEEK_SET);
    int i,j;
    for(i = 0; i < inaltime_img; i++)
    {
        for(j = 0; j < latime_img; j++)
        {
            fread(pRGB, 3, 1, fout);
            aux = 0.299*pRGB[2] + 0.587*pRGB[1] + 0.114*pRGB[0];
            pRGB[0] = pRGB[1] = pRGB[2] = aux;
            fseek(fout, -3, SEEK_CUR);
            fwrite(pRGB, 3, 1, fout);
            fflush(fout);
        }
        fseek(fout,padding,SEEK_CUR);
    }
    fclose(fout);
}
unsigned char** matrice_sablon(char *nume,int w,int h)
{
    int i,j;
    unsigned char pRGB[3];
    FILE *f= fopen(nume , "rb");
    fseek(f,54,SEEK_SET);
    int padding=3;
    unsigned char **L=(unsigned char**)malloc(h*sizeof(unsigned char*));
    for(i=0; i<h; i++)
        L[i]=(unsigned char*)malloc(w*sizeof(unsigned char));
    for(i=0; i<h; i++)
    {
        for(j=0; j<w; j++)
        {
            fread(pRGB, 3, 1, f);
            L[i][j]=pRGB[0];
        }
        fseek(f,padding,SEEK_CUR);
    }
    fclose(f);
    return L;
}
void matrice_imagine(int w,int h,unsigned char **L)
{
    int i,j;
    unsigned char pRGB[3];
    FILE *f= fopen("test_grayscale.bmp" , "rb");
    fseek(f,54,SEEK_SET);
    for(i=0; i<h; i++)
    {
        for(j=0; j<w; j++)
        {
            fread(pRGB, 3, 1, f);
            L[i][j]=pRGB[0];
        }
    }
    fclose(f);
}
double suma_medie_sablon(int h,int w,unsigned char **s)
{
    int i,j;
    double suma=0;
    for(i=0; i<h; i++)
        for(j=0; j<w; j++)
            suma=suma+(int)s[i][j];
    double s_mediu=suma/165;
    return s_mediu;
}
double deviatia_standard_sablon(int h,int w,unsigned char **s)
{
    int n=165,i,j;
    double suma=0;
    double media=suma_medie_sablon(h,w,s);
    for(i=0; i<h; i++)
        for(j=0; j<w; j++)
            suma=suma+((int)s[i][j]-media)*((int)s[i][j]-media);
    suma=suma/(n-1);
    return sqrt(suma);
}
double suma_medie_fI(int k,int l,int h,int w,unsigned char **s)
{
    int i,j;
    double suma=0;
    for(i=k; i<k+h; i++)
        for(j=l; j<w+l; j++)
            suma=suma+(int)s[i][j];
    double s_mediu=suma/165;
    return s_mediu;
}
double deviatia_standard_fI(int k,int l,int h,int w,unsigned char **s)
{
    int n=165,i,j;
    double suma=0;
    double media=suma_medie_fI(k,l,h,w,s);
    for(i=k; i<k+h; i++)
        for(j=l; j<l+w; j++)
            suma=suma+((int)s[i][j]-media)*((int)s[i][j]-media);
    suma=suma/(n-1);
    return sqrt(suma);
}
double cross_correlation(int x,int y,unsigned char **s,unsigned char **I)
{
    int n=165,i,j,w_sablon=11,h_sablon=15,w=500,h=400;
    double gama_s,gama_fI,p,corelatia,suma=0;
    double fI_mediu=suma_medie_fI(x,y,h_sablon,w_sablon,I);
    double s_mediu=suma_medie_sablon(h_sablon,w_sablon,s);
    gama_s=deviatia_standard_sablon(h_sablon,w_sablon,s);
    gama_fI=deviatia_standard_fI(x,y,h_sablon,w_sablon,I);
    for(i=0; i<h_sablon; i++)
        for(j=0; j<w_sablon; j++)
        {
            if(y+j>=0 && x+i>=0 && x+i<h && y+j<w)
            {
                p=1;
                p=p*((int)I[i+x][j+y]-fI_mediu);
                p=p*((int)s[i][j]-s_mediu);
                p=p/gama_s/gama_fI;
                suma=suma+p;
            }
        }
    corelatia=suma/n;
    return corelatia;
}
int template_matching(coordonate *v,unsigned char **I,unsigned char **s,float ps,int cifra)
{
    int x,y,w_sablon=11,h_sablon=15,w=500,h=400,k=0;
    for(x=0; x<h-h_sablon; x++)
        for(y=0; y<w-w_sablon; y++)
        {
            double corelatia=cross_correlation(x,y,s,I);
            if(corelatia>=ps)
            {
                v[k].a=x;
                v[k].b=y;
                v[k].c=corelatia;
                v[k].d=cifra;
                k++;
            }
        }
    return k;
}

void matrice(unsigned char **L,char *nume)
{
    FILE *fin=fopen("test.bmp","rb");
    FILE *fout = fopen(nume , "wb");
    int i,j,x, w=500,h=400,k;
    unsigned char pRGB[3];
    fseek(fin,0,SEEK_SET);
    fseek(fout,0,SEEK_SET);
    for(i = 0; i < 54; i++)
    {
        fread(&x , 1 , 1 , fin);
        fwrite(&x , 1 , 1 , fout);
    }
    fseek(fin,54,SEEK_SET);
    for(i=0; i<h; i++)
    {
        for(j=0; j<w; j++)
        {
            fread(pRGB, 3, 1, fin);
            L[i][3*j]=pRGB[0];
            L[i][3*j+1]=pRGB[1];
            L[i][3*j+2]=pRGB[2];
        }
        fseek(fin,0,SEEK_CUR);
    }
    fseek(fout, 54, SEEK_SET);
    for(i =0; i < h; i++)
        for(j=0; j<w; j++)
        {
            for(k=0; k<3; k++)
                fwrite(&L[i][3*j+k], 1, 1, fout);
        }
    fclose(fin);
    fclose(fout);
}

void deseneaza_contur(char *nume,unsigned char **L,int x,int y,int p1,int p2,int p3)
{
    int i,j,w=500,h=400,l,w_sablon=11,h_sablon=15;
    FILE *fout = fopen(nume, "wb");
    FILE *fin=fopen("test.bmp","rb");
    fseek(fin,0,SEEK_SET);
    fseek(fout,0,SEEK_SET);
    for(i = 0; i < 54; i++)
    {
        fread(&j , 1 , 1 , fin);
        fwrite(&j , 1 , 1 , fout);
    }
    fseek(fin,54,SEEK_SET);
    for(j = 0; j < w_sablon; j++)
    {
        L[x][3*(y+j)]=p1;
        L[x][3*(y+j)+1]=p2;
        L[x][3*(y+j)+2]=p3;
    }
    for(j = 0; j < w_sablon; j++)
    {
        L[x+h_sablon][3*(y+j)]=p1;
        L[x+h_sablon][3*(y+j)+1]=p2;
        L[x+h_sablon][3*(y+j)+2]=p3;
    }
    for(i=0; i<h_sablon; i++)
    {
        L[i+x][3*y]=p1;
        L[i+x][3*y+1]=p2;
        L[i+x][3*y+2]=p3;
    }
    for(i=0; i<h_sablon; i++)
    {
        L[i+x][3*(y+w_sablon)]=p1;
        L[i+x][3*(y+w_sablon)+1]=p2;
        L[i+x][3*(y+w_sablon)+2]=p3;
    }
    fseek(fout,54,SEEK_SET);
    for(i =0; i < h; i++)
        for(j=0; j<w; j++)
        {
            for(l=0; l<3; l++)
                fwrite(&L[i][3*j+l], 1, 1, fout);
        }
    fclose(fin);
    fclose(fout);

}
void eliberare_matrice(int inaltime,unsigned char **A)
{
    int i;
    for(i=0; i<inaltime; i++)
        free(A[i]);
    free(A);
}

int cmp(const void *a,const void *b)
{
    coordonate e1,e2;
    e1=*(coordonate *)a;
    e2=*(coordonate *)b;
    if(e1.c<e2.c)
        return 1;
    if(e1.c==e2.c)
        return 0;
    return -1;
}
double arie_intersectie(int x1,int y1,int x2,int y2)
{
    int w=11,h=15;
    int a=y1+w,b=x1+h;
    double arie_inter;
    if((abs)(x2-x1)>h || (abs)(y2-y1)>w)
        arie_inter=0;
    else if(x1==x2)
        arie_inter=(abs)(a-y2)*h;
    else if(y1==y2)
        arie_inter=(abs)(b-x2)*w;
    else
        arie_inter=(abs)(a-y2)*(b-x2);
    return arie_inter;
}
double suprapunere(int x1,int y1,int x2,int y2)
{
    int w=11,h=15;
    int arie_di=w*h;
    int arie_dj=w*h;
    double arie_i=arie_intersectie(x1, y1, x2, y2);
    double a=arie_di+arie_dj-arie_i;
    return arie_i/a;
}
int eliminare_non_maxime(int n,coordonate *v)
{
    int i=0,j,k;
    while(i<n-1)
    {
        j=i+1;
        while(j<n)
        {
            if(suprapunere(v[i].a, v[i].b, v[j].a, v[j].b)>0.2)
            {
                for(k=j; k<n-1; k++)
                {
                    v[k].a=v[k+1].a;
                    v[k].b=v[k+1].b;
                    v[k].c=v[k+1].c;
                    v[k].d=v[k+1].d;
                }
                n--;
            }
            else
                j++;
        }
        i++;
    }
    return n;
}
void partea_a_doua()
{
    int i;
    grayscale_image("test.bmp","test_grayscale.bmp");
    grayscale_image("cifra0.bmp","cifra0_grayscale.bmp");
    grayscale_image("cifra1.bmp","cifra1_grayscale.bmp");
    grayscale_image("cifra2.bmp","cifra2_grayscale.bmp");
    grayscale_image("cifra3.bmp","cifra3_grayscale.bmp");
    grayscale_image("cifra4.bmp","cifra4_grayscale.bmp");
    grayscale_image("cifra5.bmp","cifra5_grayscale.bmp");
    grayscale_image("cifra6.bmp","cifra6_grayscale.bmp");
    grayscale_image("cifra7.bmp","cifra7_grayscale.bmp");
    grayscale_image("cifra8.bmp","cifra8_grayscale.bmp");
    grayscale_image("cifra9.bmp","cifra9_grayscale.bmp");
    int latime=11,inaltime=15,x=0;
    int Latime=500,Inaltime=400;
    float ps=0.5;

    unsigned char **L=(unsigned char**)malloc(Inaltime*sizeof(unsigned char*));
    for(i=0; i<Inaltime; i++)
        L[i]=(unsigned char*)malloc(3*Latime*sizeof(unsigned char));
    matrice(L,"matrice_test.bmp");

    unsigned char **K=(unsigned char**)malloc(400*sizeof(unsigned char*));
    for(i=0; i<400; i++)
        K[i]=(unsigned char*)malloc(500*sizeof(unsigned char));
    matrice_imagine(500,400,K);

    coordonate *v=malloc(93*sizeof(coordonate));
    unsigned char **A= matrice_sablon("cifra0_grayscale.bmp",latime,inaltime);
    int a=template_matching(v,K,A,ps,0);
    for(i=0; i<a; i++)
        deseneaza_contur("matrice_test.bmp",L,v[i].a,v[i].b,0,0,255);
    eliberare_matrice(inaltime,A);
    coordonate *D=malloc(a*sizeof(coordonate));
    for(i=0; i<a; i++)
    {
        D[x].c=v[i].c;
        D[x].a=v[i].a;
        D[x].b=v[i].b;
        D[x].d=v[i].d;
        x++;
    }
    free(v);

    coordonate *w=malloc(1500*sizeof(coordonate));
    unsigned char **B= matrice_sablon("cifra1_grayscale.bmp",latime,inaltime);
    int  b=template_matching(w,K,B,ps,1);
    for(i=0; i<b; i++)
        deseneaza_contur("matrice_test.bmp",L,w[i].a,w[i].b,0,255,255);
    eliberare_matrice(inaltime,B);
    D=(coordonate*)realloc(D,(a+b)*sizeof(coordonate));
    for(i=0; i<b; i++)
    {
        D[x].c=w[i].c;
        D[x].b=w[i].b;
        D[x].a=w[i].a;
        D[x].d=w[i].d;
        x++;
    }
    free(w);

    coordonate *t=malloc(200*sizeof(coordonate));
    unsigned char **C= matrice_sablon("cifra2_grayscale.bmp",latime,inaltime);
    int c=template_matching(t,K,C,ps,2);
    for(i=0; i<c; i++)
        deseneaza_contur("matrice_test.bmp",L,t[i].a,t[i].b,0,255,0);
    eliberare_matrice(inaltime,C);
    D=(coordonate *)realloc(D,(a+b+c)*sizeof(coordonate));
    for(i=0; i<c; i++)
    {
        D[x].c=t[i].c;
        D[x].a=t[i].a;
        D[x].b=t[i].b;
        D[x].d=t[i].d;
        x++;
    }
    free(t);

    coordonate *q=malloc(120*sizeof(coordonate));
    unsigned char **M= matrice_sablon("cifra3_grayscale.bmp",latime,inaltime);
    int d=template_matching(q,K,M,ps,3);
    for(i=0; i<d; i++)
        deseneaza_contur("matrice_test.bmp",L,q[i].a,q[i].b,255,255,0);
    eliberare_matrice(inaltime,M);
    D=(coordonate *)realloc(D,(a+b+c+d)*sizeof(coordonate));
    for(i=0; i<d; i++)
    {
        D[x].c=q[i].c;
        D[x].a=q[i].a;
        D[x].b=q[i].b;
        D[x].d=q[i].d;
        x++;
    }
    free(q);

    coordonate *r=malloc(650*sizeof(coordonate));
    unsigned char **E= matrice_sablon("cifra4_grayscale.bmp",latime,inaltime);
    int e=template_matching(r,K,E,ps,4);
    for(i=0; i<e; i++)
        deseneaza_contur("matrice_test.bmp",L,r[i].a,r[i].b,255,0,255);
    eliberare_matrice(inaltime,E);
    D=(coordonate *)realloc(D,(a+b+c+d+e)*sizeof(coordonate));

    for(i=0; i<e; i++)
    {
        D[x].c=r[i].c;
        D[x].a=r[i].a;
        D[x].b=r[i].b;
        D[x].d=r[i].d;
        x++;
    }
    free(r);

    coordonate *y=malloc(150*sizeof(coordonate));
    unsigned char **F= matrice_sablon("cifra5_grayscale.bmp",latime,inaltime);
    int f=template_matching(y,K,F,ps,5);
    for(i=0; i<f; i++)
        deseneaza_contur("matrice_test.bmp",L,y[i].a,y[i].b,255,0,0);
    eliberare_matrice(inaltime,F);
    D=(coordonate *)realloc(D,(a+b+c+d+e+f)*sizeof(coordonate));
    for(i=0; i<f; i++)
    {
        D[x].c=y[i].c;
        D[x].a=y[i].a;
        D[x].b=y[i].b;
        D[x].d=y[i].d;
        x++;
    }
    free(y);

    coordonate *u=malloc(95*sizeof(coordonate));
    unsigned char **G= matrice_sablon("cifra6_grayscale.bmp",latime,inaltime);
    int g=template_matching(u,K,G,ps,6);
    for(i=0; i<g; i++)
        deseneaza_contur("matrice_test.bmp",L,u[i].a,u[i].b,192,192,192);
    eliberare_matrice(inaltime,G);
    D=(coordonate *)realloc(D,(a+b+c+d+e+f+g)*sizeof(coordonate));
    for(i=0; i<g; i++)
    {
        D[x].c=u[i].c;
        D[x].a=u[i].a;
        D[x].b=u[i].b;
        D[x].d=u[i].d;
        x++;
    }
    free(u);

    coordonate *o=malloc(851*sizeof(coordonate));
    unsigned char **H= matrice_sablon("cifra7_grayscale.bmp",latime,inaltime);
    int h=template_matching(o,K,H,ps,7);
    for(i=0; i<h; i++)
        deseneaza_contur("matrice_test.bmp",L,o[i].a,o[i].b,0,140,255);
    eliberare_matrice(inaltime,H);
    D=(coordonate *)realloc(D,(a+b+c+d+e+f+g+h)*sizeof(coordonate));
    for(i=0; i<h; i++)
    {
        D[x].c=o[i].c;
        D[x].a=o[i].a;
        D[x].b=o[i].b;
        D[x].d=o[i].d;
        x++;
    }
    free(o);

    coordonate *p=malloc(115*sizeof(coordonate));
    unsigned char **I= matrice_sablon("cifra8_grayscale.bmp",latime,inaltime);
    int k=template_matching(p,K,I,ps,8);
    for(i=0; i<k; i++)
        deseneaza_contur("matrice_test.bmp",L,p[i].a,p[i].b,128,0,128);
    eliberare_matrice(inaltime,I);
    D=(coordonate *)realloc(D,(a+b+c+d+e+f+g+h+k)*sizeof(coordonate));
    for(i=0; i<k; i++)
    {
        D[x].c=p[i].c;
        D[x].a=p[i].a;
        D[x].b=p[i].b;
        D[x].d=p[i].d;
        x++;
    }
    free(p);

    coordonate *l=malloc(110*sizeof(coordonate));
    unsigned char **J= matrice_sablon("cifra9_grayscale.bmp",latime,inaltime);
    int j=template_matching(l,K,J,ps,9);
    for(i=0; i<j; i++)
        deseneaza_contur("matrice_test.bmp",L,l[i].a,l[i].b,0,0,128);
    eliberare_matrice(inaltime,J);
    D=(coordonate *)realloc(D,(a+b+c+d+e+f+g+h+k+j)*sizeof(coordonate));
    for(i=0; i<j; i++)
    {
        D[x].c=l[i].c;
        D[x].a=l[i].a;
        D[x].b=l[i].b;
        D[x].d=l[i].d;
        x++;
    }
    free(l);
    eliberare_matrice(Inaltime,L);
    eliberare_matrice(Inaltime,K);
    qsort(D,x-1,sizeof(coordonate),cmp);

    unsigned char **X=(unsigned char**)malloc(Inaltime*sizeof(unsigned char*));
    for(i=0; i<Inaltime; i++)
        X[i]=(unsigned char*)malloc(3*Latime*sizeof(unsigned char));
    matrice(X,"imagine_finala.bmp");
    int n=a+b+c+d+e+f+g+h+k;

    k=eliminare_non_maxime(n,D);
    for(i=0; i<k; i++)
    {
        if(D[i].d==0)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,0,0,255);
        if(D[i].d==1)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,0,255,255);
        if(D[i].d==2)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,0,255,0);
        if(D[i].d==3)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,0,0,255);
        if(D[i].d==4)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,255,0,255);
        if(D[i].d==5)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,255,0,0);
        if(D[i].d==6)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,192,192,192);
        if(D[i].d==7)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,0,140,255);
        if(D[i].d==8)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,128,0,128);
        if(D[i].d==9)
            deseneaza_contur("imagine_finala.bmp",X,D[i].a,D[i].b,0,0,128);
    }
    eliberare_matrice(Inaltime,X);
}
int main()
{
    partea_a_doua();
    return 0;
}
