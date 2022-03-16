#include <stdio.h>
#include <stdlib.h>
typedef enum fig {P, W, S, G, K, Q, BF}figura; // figura (pion/wieza/skoczek/goniec/krol/hetman/brak)
typedef enum kol {B,C, BK}kolor; // kolor figury (biala/czarna/brak)
typedef struct pole{
    figura f;
    kolor k;
}pole;
typedef struct plansza{
    pole p[8][8];
}plansza;
int inside(int x, int y){
    return (x>=0&&x<8)&&(y>=0&&y<8) ? 1 : 0;
}
typedef struct para{// wspolrzedne pola
    int x;
    int y;
}para;
typedef struct ruch{
    para z_pola;
    para na_pole;
}ruch;
const ruch zero;
typedef struct bicie_w_p{
    int czy;
    ruch r;
}bicie_w_p;
const bicie_w_p ze = {1,{{0,0},{0,0}}};
typedef struct stan_gry{
    kolor czyj_ruch;
    int czy_pod_szachem;
    bicie_w_p bwp1;
    bicie_w_p bwp2;
    int mozliwosc_roszady_bialych_prawostronna;
    int mozliwosc_roszady_bialych_lewostronna;
    int mozliwosc_roszady_czarnych_prawostronna;
    int mozliwosc_roszady_czarnych_lewostronna;
}stan_gry;
plansza promocja(plansza p1, int x, int y, figura f){
    p1.p[x][y].f=f;
    return p1;
}
typedef struct list{
    struct list* kolejny;
    struct list* koniec;
    ruch r;
}list;

list *pb(list *l1, ruch r1){
    list *n = calloc(1,sizeof(list));
    n->r = r1;
    n->koniec=n;
    list *ostatni= l1->koniec;
    ostatni->kolejny=n;
    l1->koniec = n;
    return l1;
}
list *pof(plansza p1,int x, int y,int i, int j, int maks, list *glowa){
    for(int a=1; a<maks; a++){
        if(inside(x+a*i,y+a*j)){
            if(p1.p[x+a*i][y+a*j].f==BF){
                ruch r1;
                r1.z_pola.x=x; r1.z_pola.y = y;
                r1.na_pole.x = x+a*i; r1.na_pole.y = y+a*j;
                glowa = pb(glowa,r1);
            }
            else{
                if(p1.p[x+a*i][y+a*j].k!=p1.p[x][y].k){
                    ruch r1;
                    r1.z_pola.x=x; r1.z_pola.y = y;
                    r1.na_pole.x = x+a*i; r1.na_pole.y = y+a*j;
                    glowa=pb(glowa,r1);
                }
                break;
            }
        }
    }
    return glowa;
}
list *rpion(plansza p1, list *glowa, int a, int x, int y, stan_gry STS){
    for (int i=-1; i<2; i+=2) if(inside(x+a, y+i)) if(p1.p[x+a][y+i].f!=BF&&p1.p[x+a][y+i].k!=p1.p[x][y].k){
        ruch bp={{x,y},{x+a,y+i}};

        glowa = pb(glowa,bp);
    }
    if(inside(x+a,y)) if(p1.p[x+a][y].f==BF){
        ruch r1={{x,y},{x+a,y}};
        glowa = pb(glowa,r1);
        if((x==6&&a==-1)||(x==1&&a==1)){
            if(p1.p[x+2*a][y].f==BF){
                ruch r2 = {{x,y},{x+2*a,y}};
                list *sp =glowa->koniec;
                int ik=(p1.p[x][y].k==B ? (-1) : 1);
                glowa = pb(glowa,r2);
                if(sp!=glowa->koniec){
                    if(inside(x+2*a,y-1)){
                        if(p1.p[x+2*a][y-1].f==P&&p1.p[x+2*a][y-1].k!=p1.p[x][y].k){
                            ruch r1={{x+2*a,y-1},{x+a,y}};
                            STS.bwp1.r=r1;
                            STS.bwp1.czy=1;
                        }
                    }
                    if(inside(x+2*a,y+1)){
                        if(p1.p[x+2*a][y+1].f==P&&p1.p[x+2*a][y+1].k!=p1.p[x][y].k){
                            ruch r1={{x+2*a,y+1},{x+a,y}};
                            STS.bwp1.r=r1;
                            STS.bwp1.czy=1;
                        }
                    }
                }
            }
        }
    }
    if(STS.bwp1.czy==1){
        glowa = pb(glowa,STS.bwp1.r);
    }
    if(STS.bwp2.czy==1){
        glowa = pb(glowa,STS.bwp2.r);
    }
    return glowa;
}
list *generator_ruchow(plansza p1, kolor k1, stan_gry STS){ // generuje wszystkie ruchy, rowniez te nielegalne
    list *pocz = calloc(1,sizeof(list));
    pocz->kolejny=NULL;
    int tab[4]={-2,-1,1,2};
    pocz->koniec = pocz;
    pocz->r=zero;// ruch zerowy nie majacy wplywu na nic ale symbolizuje poczatek listy i do niego mozna dodawac kolejne
    for(int x=0; x<8; x++){
        for(int y=0; y<8; y++){
            if(p1.p[x][y].k!=k1) continue;
            switch (p1.p[x][y].f)
            {
            case P:
                if(k1==B){
                    pocz = rpion(p1, pocz, 1,x,y, STS);
                }
                if(k1==C){
                    pocz = rpion(p1,pocz,-1,x,y, STS);
                }
                break;
            case W:
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        if(abs(i)==abs(j))continue;
                        pocz = pof(p1,x,y,i,j,8,pocz);
                    }
                }
                break;
            case S:
                for(int i=0; i<4; i++){
                    for(int j=0; j<4; j++){
                        if(abs(tab[i])==abs(tab[j])) continue;
                        if(inside(x+tab[i],y+tab[j])){
                            if(p1.p[x+tab[i]][y+tab[j]].f==BF){
                                ruch rs={{x,y},{x+tab[i],y+tab[j]}};
                                pocz = pb(pocz,rs);
                            }
                            else{
                                if(p1.p[x+tab[i]][y+tab[j]].k!=k1){
                                    ruch rs={{x,y},{x+tab[i],y+tab[j]}};
                                    pocz = pb(pocz,rs);
                                }
                            }
                        }
                    }
                }
                break;
            case G:
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        if(i==0||j==0)continue;
                        pocz = pof(p1,x,y,i,j,8,pocz);
                    }
                }
                break;
            case Q:
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        if(i==0||j==0)continue;
                        pocz = pof(p1,x,y,i,j,8,pocz);
                    }
                }
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        if(abs(i)==abs(j))continue;
                        pocz = pof(p1,x,y,i,j,8,pocz);
                    }
                }
                break;
            case K:
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        if(i==0&&j==0)continue;
                        pocz = pof(p1,x,y,i,j,2,pocz);
                    }
                }
            default:
                break;
            }
        }
    }
    return pocz;
}
plansza wykonaj_ruch(plansza p1, ruch r1, pole *zbita_figura){
    if(p1.p[r1.z_pola.x][r1.z_pola.y].f==K){
        int a = r1.z_pola.y;
        int b = r1.na_pole.y;
        int pw, rz;
        if(a-b==2) pw=0;
        if(b-a==2) pw=7;
        if(p1.p[r1.z_pola.x][r1.z_pola.y].k==B) rz=0;
        else rz = 7;
        p1.p[rz][b].k=p1.p[rz][a].k; p1.p[rz][b].f=K;
        p1.p[rz][(a+b)/2].f=W; p1.p[rz][(a+b)/2].k=p1.p[rz][b].k;
        p1.p[r1.z_pola.x][r1.z_pola.y].k=BK; p1.p[r1.z_pola.x][r1.z_pola.y].f=BF;
        p1.p[rz][pw].k=BK; p1.p[rz][pw].f=BF;
    }
    *zbita_figura=p1.p[r1.na_pole.x][r1.na_pole.y];
    p1.p[r1.na_pole.x][r1.na_pole.y]=p1.p[r1.z_pola.x][r1.z_pola.y];
    p1.p[r1.z_pola.x][r1.z_pola.y].k=BK; p1.p[r1.z_pola.x][r1.z_pola.y].f=BF;
    return p1;
}
para pozycja_krola(plansza p1,kolor k1){
    para p;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(p1.p[i][j].f==K&&p1.p[i][j].k==k1){
                p.x=i;
                p.y=j;
                return p;
            }
        }
    }
}
int czy_szach(plansza p1, kolor k1, stan_gry STS){
    para poz_k =pozycja_krola(p1,k1);
    int x=poz_k.x;
    int y=poz_k.y;
    kolor przeciwny = (p1.p[x][y].k==B ? C : B);
    list *ruchy_przeciwnika = generator_ruchow(p1,przeciwny,STS);
    list *aktualny= ruchy_przeciwnika->kolejny;
    while(aktualny->kolejny!=NULL){
        if(aktualny->r.na_pole.x==x && aktualny->r.na_pole.y == y){
            return 1;
        }
        aktualny=aktualny->kolejny;
    }
    if(aktualny->r.na_pole.x==x && aktualny->r.na_pole.y == y){
        return 1;
    }
    return 0;
}
void wypisz(plansza p1){
    int g=41;
    printf("  ");
    for(int i=1; i<9; i++){
        printf( "  %c  ",64+i);
    }
    printf("\n");
    printf("  ");
    for(int i=0; i<g; i++) printf("_");
    printf("\n");
    for(int x=7; x>=0; x--,printf("\n",x+1)){
        printf("%d | ",x+1);
        for(int y=0; y<8; y++,printf("  | ")){
           if(p1.p[x][y].k==B){
               switch (p1.p[x][y].f)
               {
                case K:
                   printf("♔");
                   break;
                case Q:
                   printf("♕");
                   break;
                case W:
                   printf("♖");
                   break;
                case G:
                   printf("♗");
                   break;
                case S:
                   printf("♘");
                   break;
                case P:
                   printf("♙");
                   break;
                default:
                    printf(" ");
                   break;
               }
           }
           else{
               switch (p1.p[x][y].f)
               {
                case K:
                   printf("♚");
                   break;
                case Q:
                   printf("♛");
                   break;
                case W:
                   printf("♜");
                   break;
                case G:
                   printf("♝");
                   break;
                case S:
                   printf("♞");
                   break;
                case P:
                   printf("♟︎");
                   break;

                default:
                printf(" ");
                   break;
               }
           }
        }
        printf("\n");
        printf("  ");
        for(int i=0; i<g; i++) printf("⎼");
    }
    printf("  ");
    for(int i=1; i<9; i++){
        printf( "  %c  ",64+i);
    }
    printf("\n");

}
plansza plansza_startowa(){
    plansza p1;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(i<2) p1.p[i][j].k=B;
            if(i>1&&i<6){
                p1.p[i][j].k=BK;
                p1.p[i][j].f=BF;
            }
            if(i>5) p1.p[i][j].k=C;
            if(i<1||i>6){
                switch (j)
                {
                case 0:
                case 7:
                    p1.p[i][j].f=W;
                    break;
                case 1:
                case 6:
                    p1.p[i][j].f=S;
                    break;
                case 2:
                case 5:
                    p1.p[i][j].f=G;
                    break;
                case 3:
                    p1.p[i][j].f=Q;
                    break;
                case 4:
                    p1.p[i][j].f=K;
                    break;
                default:
                    break;
                }
            }
            if(i==1||i==6) p1.p[i][j].f=P;
        }
    }
    return p1;
}
stan_gry poczatkowy_stan_gry(){
    stan_gry STS;
    STS.bwp1.czy=0; STS.bwp2.czy=0;
    STS.czy_pod_szachem=0; STS.czyj_ruch=B;
    STS.mozliwosc_roszady_bialych_lewostronna=1; STS.mozliwosc_roszady_bialych_prawostronna=1;
    STS.mozliwosc_roszady_czarnych_lewostronna=1; STS.mozliwosc_roszady_czarnych_prawostronna=1;
    return STS;
}
plansza pusta(){
    plansza p1;
    for(int i=0; i<8; i++) for(int j=0; j<8; j++){
        p1.p[i][j].f=BF;
        p1.p[i][j].k=BK;
    }
    return p1;
}
list *generator_legalnych_ruchow(plansza p1,kolor k1,stan_gry STS){
    list *podstawowa = generator_ruchow(p1,k1, STS);
    list *akt = podstawowa->kolejny;
    list *poprzedni = podstawowa;
    while(akt->kolejny!=NULL){
        plansza *p2 = calloc(1,sizeof(plansza));
        *p2 = p1;
        pole zbf;
        *p2=wykonaj_ruch(*p2, akt->r, &zbf);
        if(czy_szach(*p2,k1,STS)==1){
            poprzedni->kolejny=akt->kolejny;
            list *temp = akt;
            akt = akt->kolejny;
            free(temp);
        }
        else{
            poprzedni=akt;
            akt=akt->kolejny;
        }
        free(p2);
    }
    plansza *p2 = calloc(1,sizeof(plansza));
    *p2 = p1;
    pole zbf;
    *p2=wykonaj_ruch(*p2, akt->r,&zbf);
    if(czy_szach(*p2,k1,STS)==1){
        poprzedni->kolejny=NULL;
        free(akt);
    }
    free(p2);
    return podstawowa;
}
int pat_mat(list *glowa, stan_gry STS){/// 0-gra toczy sie dalej   1-pat    2-mat
    if(glowa->kolejny==NULL){
        if(STS.czy_pod_szachem==1) return 2;
        else return 1;
    }
    return 0;
}
int rozmiar_listy(list* glowa){
    if(glowa->kolejny==NULL) return 0;
    list *akt = glowa->kolejny;
    int i=1;
    while(akt->kolejny!=NULL){
        i++;
        akt=akt->kolejny;
    }
    return i;
}
stan_gry zauktualizuj_stan_gry(plansza p1,stan_gry STS){
    if(p1.p[0][4].f!=K){
        STS.mozliwosc_roszady_bialych_lewostronna=0;
        STS.mozliwosc_roszady_bialych_prawostronna=0;
    }
    if(p1.p[7][4].f!=K){
        STS.mozliwosc_roszady_czarnych_lewostronna=0;
        STS.mozliwosc_roszady_czarnych_prawostronna=0;
    }
    if( p1.p[0][0].f!=W) STS.mozliwosc_roszady_bialych_lewostronna=0;
    if(p1.p[7][0].f!=W) STS.mozliwosc_roszady_czarnych_lewostronna=0;
    if(p1.p[7][7].f!=W) STS.mozliwosc_roszady_czarnych_prawostronna=0;
    if(p1.p[0][7].f!=W) STS.mozliwosc_roszady_bialych_prawostronna=0;
    STS.czyj_ruch= (STS.czyj_ruch==B ? C:B);
    STS.czy_pod_szachem=czy_szach(p1,STS.czyj_ruch, STS);
    return STS;
}
const int mat = 10000000;
const int pat = 0;
int ocen_pozycje(plansza p1, stan_gry STS){
    int d=5;
    int p=1*d;
    int s=3*d;
    int g=3*d;
    int w=5*d;
    int h=9*d;
    int fb=0; int fc=0; // figury bialych/czarnych
    list *ruchy=generator_ruchow(p1,STS.czyj_ruch,STS);
    //list *rc=generator_ruchow(p1,C,STS);
    int rozru=rozmiar_listy(ruchy);
    int ab=0;
    int ac=0;
    int wartosci[8][8]={
        {1,1,1,1,1,1,1,1},
        {1,2,2,2,2,2,2,1},
        {1,2,3,3,3,3,2,1},
        {1,2,3,4,4,3,2,1},
        {1,2,3,4,4,3,2,1},
        {1,2,3,3,3,3,2,1},
        {1,2,2,2,2,2,2,1},
        {1,1,1,1,1,1,1,1},
    };
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            int w;
            w=wartosci[i][j];
            switch (p1.p[i][j].k)
            {
            case B:
                ab+=w;
                break;
            case C:
                ac+=w;
                break;
            default:
                break;
            }
        }
    }
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            int v=0;
            switch (p1.p[i][j].f)
            {
            case P:
                v=p;
                break;
            case S:
                v=s;
                break;
            case G:
                v=g;
                break;
            case W:
                v=w;
                break;
            case Q:
                v=h;
                break;
            default:
                break;
            }
            if(p1.p[i][j].k==B) fb+=v;
            if(p1.p[i][j].k==C) fc+=v;
        }
    }
    int dla = (STS.czyj_ruch==B ? 1 : -1);
    return (fb-fc+ab-ac)*dla;
}

plansza odkonaj_ruch(plansza p1, ruch r1, pole zfig){
    p1.p[r1.na_pole.x][r1.na_pole.y]=p1.p[r1.z_pola.x][r1.z_pola.y];
    p1.p[r1.z_pola.x][r1.z_pola.y]=zfig;
    return p1;
}
int szukaj(int glebokosc, plansza p1, stan_gry STS, int alfa, int beta){
    if(glebokosc==0){
        return ocen_pozycje(p1,STS);
    }
    list *lr = generator_legalnych_ruchow(p1, STS.czyj_ruch, STS);
    int a=pat_mat(lr,STS);
    if(a==1){
        return pat;
    }
    if(a==2){
        return -mat;
    }
    int rozmiar=rozmiar_listy(lr);
    list *akt= lr->kolejny;
    while(rozmiar){
        pole zbf;
        p1=wykonaj_ruch(p1,akt->r,&zbf);
        stan_gry STS1 = zauktualizuj_stan_gry(p1,STS);
        int ocena=-szukaj(glebokosc-1, p1, STS1, -alfa, -beta);
        ruch temp;
        temp.z_pola=akt->r.na_pole;
        temp.na_pole=akt->r.z_pola;
        p1=odkonaj_ruch(p1, temp, zbf);
        rozmiar--;
        akt=akt->kolejny;
        if(ocena>=beta){
            return beta;
        }
        if(ocena>alfa) alfa=ocena;
    }
    return alfa;
}
typedef struct plansza_ocena{
    ruch r;
    int ocena;
}PO;
plansza wstaw_f(char a[], int x, int y,plansza p1){
    switch (a[0])
    {
    case 'b':
        p1.p[x][y].k=B;
        break;
    case 'c':
        p1.p[x][y].k=C;
        break;;
    default:
        break;
    }
    switch (a[1])
    {
    case 'p':
        p1.p[x][y].f=P;
        break;
    case 'q':
        p1.p[x][y].f=Q;
        break;
    case 'k':
        p1.p[x][y].f=K;
        break;
    case 'w':
        p1.p[x][y].f=W;
        break;
    case 's':
        p1.p[x][y].f=S;
        break;
    case 'g':
        p1.p[x][y].f=G;
        break;
    default:
        break;
    }
    return p1;

}
plansza przewin_liste(plansza p1, char a[]){
    int j=0;
    while(a[j]!=EOF) j++;
    int n=j/4;
    for(int i=0; i<n; i++){
        char b[2];
        b[0]=a[4*i];
        b[1]=a[4*i+1];
        int x = (int)a[4*i+3]-49;
        int y = (int)a[4*i+2]-97;
        p1 = wstaw_f(b,x,y,p1);
    }
    return p1;
}

void ruch_komputera(plansza *Plansza_Glowna, stan_gry *STS){
    plansza pl = *Plansza_Glowna;
    kolor tb = STS->czyj_ruch;
    stan_gry tm = *STS;
    list *glowa = generator_legalnych_ruchow(pl, tb,tm);
    int roz = rozmiar_listy(glowa);
    list *akt = glowa->kolejny;
    PO najlepszy;

    najlepszy.ocena = mat + 1;
    for(int i=0; i<roz; i++){
        pole zbf;
        pl = wykonaj_ruch(pl,akt->r,&zbf);
        stan_gry STS1=tm;
        STS1=zauktualizuj_stan_gry(pl,STS1);
        int ocena=szukaj(4,pl,STS1, -mat,mat);
        if(ocena<najlepszy.ocena){
            najlepszy.ocena=ocena;
            najlepszy.r=akt->r;
        }
        ruch temp;
        temp.z_pola=akt->r.na_pole;
        temp.na_pole=akt->r.z_pola;
        pl = odkonaj_ruch(pl,temp,zbf);
        akt=akt->kolejny;
    }
    pole zbf;
    pl = wykonaj_ruch(pl,najlepszy.r,&zbf);
    tm=zauktualizuj_stan_gry(pl,tm);
    *Plansza_Glowna = pl;
    *STS = tm;
}

void main(){
    //// ustawianie planszy i stanu gry
    plansza Plansza_Glowna = plansza_startowa();
    stan_gry Glowny_Stan_Gry = poczatkowy_stan_gry();
    ////
    wypisz(Plansza_Glowna);
    for(int i=0; i<10; i++){
        ruch_komputera(&Plansza_Glowna, &Glowny_Stan_Gry);
        wypisz(Plansza_Glowna);
    }
}
