#include "graphics.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cmath>

using namespace genv;
using namespace std;

const int XX = 600;
const int YY = 400;
const int timer = 200;
const int szelesseg = 25;
const int kezdeti_kigyohossz = 4;


void Clr(){
    gout<<move_to(0,0)<<color(0,0,0)<<box(XX,YY);
}

void Fomenut_kirajzol(){
    Clr();
    gout<<color(0, 120, 0) << move_to(2*XX/5, YY/3)          << box_to(3*XX/5, YY/3+2*YY/5/3)
        <<color(255, 100,  50) << move_to(2*XX/5, YY/3+3*YY/5/3) << box_to(3*XX/5, 2*YY/3)
        <<color(  255, 255,   255) << move_to((2*XX/5+3*XX/5)/2-20, (YY/3+YY/3+2*YY/5/3)/2) << text("PLAY")
        <<color(255, 255,   255) << move_to((2*XX/5+3*XX/5)/2-20, (YY/3+3*YY/5/3+2*YY/3)/2) << text("QUIT")
        <<move_to(10,YY-5)<<text("Pressing ENTER is also working to start. Press esc to exit.")

        <<refresh;
}


struct egyseg{
    int x, y;
    int szel; //szelesseg
    unsigned char r, g, b;

    egyseg(){
        x=szelesseg*(rand()%(XX/szelesseg));
        y=szelesseg*(rand()%(YY/szelesseg));
        szel=szelesseg;
        r=255;
        g=255;
        b=255;
    };

    egyseg(int _x, int _y, int _szel){
        x=_x;
        y=_y;
        szel=_szel;
        r=rand()%255;
        g=rand()%255;
        b=rand()%255;
    }

    void Rajzol(){
        if(x>=0 && x+szel<=XX && y>=0 && y+szel<=YY){
            gout<<move_to(x,y)<<color(r,g,b)<<box(szel,szel);
        }
    }

    void Szinvalt(){
        r=rand()%255;
        g=rand()%255;
        b=rand()%255;
    }
};


class Fal{
public:
    vector<egyseg> teglak;

    void Hozzaad(int kezd_x, int kezd_y, int veg_x, int veg_y){
        int _szel=szelesseg;
        if(kezd_x!=veg_x && kezd_y!=veg_y) cerr<<"A falhozzaadasnal az x vagy az y koordinataknak egyenloknek kell lennie!";
        if((kezd_x==veg_x && kezd_y>veg_y) || (kezd_y==veg_y && kezd_x>veg_x)) cerr<< "A falhozzaadasnal eloszor irdd a kisebb koordinatat!";
        if((kezd_x)%_szel!=0 || (kezd_y)%_szel!=0 || (veg_x)%_szel!=0 || (veg_x)%_szel!=0) cerr<<"A fal letrehozasanal a koordinataknak oszthatoknak kell lenniuk a szelessegggel!";

        int elozo_x = kezd_x-_szel;
        int elozo_y = kezd_y-_szel;

        if(kezd_y==veg_y)
            for(unsigned int i = kezd_x; i<=veg_x; i+=_szel){
                egyseg tegys(elozo_x+_szel, kezd_y, _szel);
                tegys.r= 150; tegys.g= 150; tegys.b= 150;
                teglak.push_back(tegys);
                elozo_x = tegys.x;
            }
        if(kezd_x==veg_x)
            for(unsigned int i = kezd_y; i<=veg_y; i+=_szel){
                egyseg tegys(kezd_x, elozo_y+_szel, _szel);
                tegys.r= 150; tegys.g= 150; tegys.b= 150; tegys.szel=_szel;
                teglak.push_back(tegys);
                elozo_y = tegys.y;
            }
    }

    void Rajzol(){
        for(egyseg i : teglak){
            gout<<move_to(i.x, i.y)<<color(i.r, i.g, i.b)<<box(i.szel, i.szel);
        }
    }

};


class Kigyo{
public:
    egyseg fej;
    vector<egyseg> test;
    vector<egyseg> falatok;
    int elozo_x;
    int elozo_y;
    char irany; //l/f/j/b
    int pontszam;
    bool game_over;

    Kigyo(){};

    Kigyo(int hossz, int _szel){
        irany='f';
        pontszam=0;
        game_over = false;

        egyseg tfej(XX/2, YY/2, _szel);
        fej = tfej;
        egyseg elozo (fej.x, fej.y, _szel);
        for(unsigned int i=0; i<hossz; i++){
            egyseg tegys(elozo.x, elozo.y+_szel, _szel);
            test.push_back(tegys);
            elozo.y = tegys.y;
        }
    }

    void Rajzol(){
        fej.Rajzol();
        for(auto i : test){
            i.Rajzol();
        }
        for(auto i : falatok){
            i.Rajzol();
        }
    }

    void Mozog(){
        elozo_x = fej.x;
        elozo_y = fej.y;

        if(irany=='j') fej.x+=fej.szel;
        if(irany=='b') fej.x-=fej.szel;
        if(irany=='l') fej.y+=fej.szel;
        if(irany=='f') fej.y-=fej.szel;

        if(fej.x<0) fej.x+=XX;
        if(fej.y<0) fej.y+=YY;
        if(fej.x>=XX) fej.x-=XX;
        if(fej.y>=YY) fej.y-=YY;

        for(auto &i : test){
            int tmpx=i.x;
            int tmpy=i.y;
            i.x = elozo_x;
            i.y = elozo_y;
            elozo_x = tmpx;
            elozo_y = tmpy;
        }
    }

    void Iranyvalt(char ujirany){
        if((ujirany=='f' && irany!='l') || (ujirany=='l' && irany!='f') || (ujirany=='j' && irany!='b') || (ujirany=='b' && irany!='j')) irany = ujirany;
    }

    bool Hamm(egyseg kaja){
        if(fej.x==kaja.x && fej.y==kaja.y) return true;
        return false;
    }

    egyseg Uj_kaja_generalo(Fal _fal){
        egyseg ujkaja;
      ///ne legyen a kigyoban
        while(ujkaja.x==fej.x && ujkaja.y==fej.y){
            egyseg ujabbkaja;
            ujkaja = ujabbkaja;
        }
        for(unsigned int i = 0; i<test.size(); i++){
            if(ujkaja.x==test[i].x && ujkaja.y==test[i].y){
                egyseg ujabbkaja;
                ujkaja = ujabbkaja;
                i=0;
            }
        }
      ///ne legyen falban
        for(unsigned int i = 0; i<_fal.teglak.size(); i++){
            if(ujkaja.x==_fal.teglak[i].x && ujkaja.y==_fal.teglak[i].y){
                egyseg ujabbkaja;
                ujkaja = ujabbkaja;
                i=0;
            }
        }

        return ujkaja;
    }

    void Novel(){
        egyseg ujegyseg(elozo_x, elozo_y, szelesseg);
        test.push_back(ujegyseg);
    }

    void Pontszam_kiir(){
        stringstream ss;
        string spontszam;
        ss<<pontszam;
        ss>>spontszam;
        gout<<move_to(XX-30, 30)<<color(0,255,0)<<text(spontszam);
    }

    bool Paused(bool paused){

        if(paused)
            gout<<color(255,255,255)<<move_to(XX/2-25, YY/2)<<text("Paused")
                <<color(170, 170, 250)<<move_to(2*XX/5, 4*YY/7)<<box_to(3*XX/5, 5*YY/7)
                <<color(240, 50, 0)<<move_to((2*XX/5+3*XX/5)/2-17, (4*YY/7+5*YY/7)/2)<<text("MENU");

        return paused;
    }

    bool Game_Over(Fal _fal, egyseg _fej, int _time_szamlalo){
      ///onmagaba utkozik
        for(egyseg i : test) if(fej.x==i.x && fej.y==i.y){game_over = true; break;}
      ///falba utkozik
        for(egyseg i : _fal.teglak) if(fej.x==i.x && fej.y==i.y) {game_over = true; break;}

        if(!game_over) return false;


        if((_time_szamlalo/2)%2==0){ fej.r=0; fej.g=0; fej.b=255;}
        else  { fej.r=255; fej.g=255; fej.b=255;}
        _fej.Rajzol();

        gout<<color(255,0,0)<<move_to(XX/2-35, YY/2)<<text("Game Over");
        gout<<color(170, 170, 250)<<move_to(2*XX/5, 4*YY/7)<<box_to(3*XX/5, 5*YY/7)
            <<color(240, 50, 0)<<move_to((2*XX/5+3*XX/5)/2-17, (4*YY/7+5*YY/7)/2)<<text("MENU");

        return true;
    }
};
///___________________________________________________________________________________________________________________________________________
int main(){
    srand(time(0));
    gout.open(XX,YY);
    event ev;
    gin.timer(timer);
    bool menu = true;
    bool esc = false;
    bool paused;
    int time_szamlalo;

    Kigyo kigyo;
    vector<int> emesztes;
    Fal fal;
    fal.Hozzaad(100, 0.5*(XX/4-(XX/4)%25), XX/2-25, 0.5*(XX/4-(XX/4)%25));
    fal.Hozzaad(XX/2+25, 0.5*(XX/4-(XX/4)%25), XX-100, 0.5*(XX/4-(XX/4)%25));
    fal.Hozzaad(100, 2*(XX/4-(XX/4)%25), XX/2-25, 2*(XX/4-(XX/4)%25));
    fal.Hozzaad(XX/2+25, 2*(XX/4-(XX/4)%25), XX-100, 2*(XX/4-(XX/4)%25));
    //fal.Hozzaad(XX/2+25, 0, XX/2+25, YY);
    //fal.Hozzaad(0, YY/2+50, XX, YY/2+50);
    egyseg kaja;

    while(gin >> ev && (ev.button!=btn_left || (ev.pos_x<2*XX/5 || ev.pos_x>3*XX/5 || ev.pos_y<YY/3+3*YY/5/3 || ev.pos_y>2*YY/3)) && ev.keycode!=key_escape && !esc){

        Fomenut_kirajzol();

        if(ev.button==btn_left && ev.pos_x>=2*XX/5 && ev.pos_x<=3*XX/5 || ev.keycode==key_enter){
                menu = false;
                paused = false;
                Kigyo tkigyo(kezdeti_kigyohossz-1, szelesseg);
                kigyo = tkigyo;
                kaja = kigyo.Uj_kaja_generalo(fal);
                emesztes.clear();
                time_szamlalo=0;
        }

        while(gin >> ev && !menu && !esc){
            if(ev.time){
                    //cout<<paused;
                if(!kigyo.Game_Over(fal, kigyo.fej, time_szamlalo) && !kigyo.Paused(paused)){
                    Clr();

                    for(unsigned int i=0; i<emesztes.size(); i++){
                        if(emesztes[i]==kigyo.test.size()){
                        kigyo.Novel();
                        egyseg tegys(-1,-1,0);
                        kigyo.falatok[i] = tegys;
                        emesztes[i]=-1;

                        }

                        if(emesztes[i]!=-1){
                            emesztes[i]++;
                        }
                    }

                    kigyo.Mozog();


                    fal.Rajzol();
                    kaja.Szinvalt();
                    kaja.Rajzol();
                    kigyo.Rajzol();
                    kigyo.Pontszam_kiir();

                    kigyo.Game_Over(fal, kigyo.fej, time_szamlalo); //nem lenne szukseges, csak azet raktam be, hogy rogton kiirja a Game Over-t
                }

                gout<<refresh;
                time_szamlalo++;
            }

            if(ev.keycode==key_up) kigyo.Iranyvalt('f');
            if(ev.keycode==key_down) kigyo.Iranyvalt('l');
            if(ev.keycode==key_right) kigyo.Iranyvalt('j');
            if(ev.keycode==key_left) kigyo.Iranyvalt('b');

            if(kigyo.Hamm(kaja)){
                    egyseg tegys (kigyo.fej.x-2, kigyo.fej.y-2, szelesseg+4);

                    bool nincs_ures = true;
                    for(unsigned int i=0; i<emesztes.size(); i++){
                        if(emesztes[i]==-1){
                            emesztes[i]=0;
                            kigyo.falatok[i] = tegys;
                            nincs_ures = false;
                            break;
                        }
                    }
                    if(nincs_ures){
                        emesztes.push_back(0);
                        kigyo.falatok.push_back(tegys);
                    }

                    kaja = kigyo.Uj_kaja_generalo(fal);
                    kigyo.pontszam++;
            }
            if(ev.keycode=='p' && !paused) paused = true;
            else if(ev.keycode=='p' && paused) paused = false;
            cout<<paused<<!kigyo.Paused(paused)<<endl;


            if(ev.button==btn_left && ev.pos_x>2*XX/5 && ev.pos_x<3*XX/5 && ev.pos_y>4*YY/7 && ev.pos_y<5*YY/7 || ev.keycode==key_enter)
                menu = true;
            if(ev.keycode==key_escape) esc = true;

        }
    }

    return 0;
}
