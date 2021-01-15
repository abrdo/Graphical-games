#include "graphics.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace genv;
using namespace std;

const int XX = 1000;
const int YY = 500;
const int timer = 10;
const float seb=1;

struct flappy
{
private:
    float x,y;
    float vy;
    vector<float> kx, ky;
    vector<float> kszel;
    vector<float> xpos, ypos;
    vector<float> pszel;
    vector<float> pszin;

    int szamlalo;
    bool go; // is game over?

public:
    flappy()
    {
        x=XX/5;
        y=YY/2;
        vy=0;
        gout<<color(255,255,255)<<move_to(x,y)<<box(3,3);
        go=false;
        szamlalo=0;
    }

    void kaput_general(int ky_, int kszel_)
    {
        kx.push_back(XX-1);
        ky.push_back(ky_);
        kszel.push_back(kszel_);
    }


    void rajzol()
    {
        gout<<color(255,255,0)<<move_to(x,y)<<box(3,3);


        for(int i=0; i<kx.size(); i++)
        {
            if(kx[i]>0)
            {
                gout<<color(225, 225, 255)<<move_to(kx[i],0)<<line_to(kx[i], ky[i]-kszel[i]/2)<<move_to(kx[i], ky[i]+kszel[i]/2)<<line_to(kx[i], YY);
            }

        }

        xpos.push_back(x);
        ypos.push_back(y);
        pszel.push_back(0);
        pszin.push_back(255);

        for(int i=0; i<xpos.size(); i++)
        {
            xpos[i]-=seb;
            pszel[i]-= 0.06*(5-(XX/5/2-xpos[i])/(XX/0.5));
            int tpszel=pszel[i];


    ///eredeti, szines:
            if(pszin[i]*(pszin[i]-225/(XX/9))>0) pszin[i]-=225/(XX/9);

    ///kukac:
            //if(1) pszin[i]-=225/(XX/100);
            else pszin[i]=0;

            if(xpos[i]>=0)
            {
    ///eredeti, kukac:
                gout<<color(pszin[i], 0.7*pszin[i], 0/*170, 170, 250*/)<<move_to(xpos[i],ypos[i])<<box(2,2);

    ///szineshez:
                //gout<<color(i*0.1, (255-pszin[i])/i*100, pszin[i]/9)<<move_to(xpos[i],ypos[i])<<box(tpszel, tpszel);
            }

        }


        ///rekord
        ifstream f("rekord.txt");
        if(!f.good()) cerr<<"Nincs meg a 'rekord.txt' fajl!"<<endl;
        int rek;
        f>>rek;
        if(rek<szamlalo)
        {
            rek=szamlalo;
            ofstream of("rekord.txt");
            of<<szamlalo;
        }

        string s;
        stringstream ss;
        ss<<szamlalo;
        ss>>s;
        gout<<move_to(XX-300, 30)<<color(0 , 255, 255)<<text(s);
        ss.clear();
        ss<<rek;
        ss>>s;
        gout<<move_to(XX-200,30 )<<color(255 , 0, 130)<<text("rekord: "+s);

    }


    void mozog(float zuhanas)
    {
        y+=vy;
//        x+=0.4;
        vy+=zuhanas;
        for(int i=0; i<kx.size(); i++)
        {
            kx[i]-=seb;
        }

    }

    void repul(float mennyire)
    {
        vy-=mennyire/100;
    }

    void szamlalot_novel()
    {
        for(int i=0; i<kx.size(); i++) if(kx[i]==XX/5 && (y>ky[i]-kszel[i]/2 && y<ky[i]+kszel[i]/2)) szamlalo++;
    }

    bool g_o() // game over
    {
        for(int i=0; i<kx.size(); i++) if(kx[i]==XX/5 && (y<=ky[i]-kszel[i]/2 || y>=ky[i]+kszel[i]/2)) go=true;

        if(y>YY || y<0) go=true;

        return go;
    }

};

///----------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    srand((int) time(nullptr));
    gout.open(XX, YY);
    event ev;
    gin.timer(timer);


    while (gin >> ev && (ev.button!=btn_left || (ev.pos_x<2*XX/5 || ev.pos_x>3*XX/5 || ev.pos_y<YY/3+3*YY/5/3 || ev.pos_y>2*YY/3)))
    {
        gout<<color(170, 170, 250) << move_to(2*XX/5, YY/3)          << box_to(3*XX/5, YY/3+2*YY/5/3)
            <<color(255, 100,  50) << move_to(2*XX/5, YY/3+3*YY/5/3) << box_to(3*XX/5, 2*YY/3)
            <<color(  0, 100,   0) << move_to((2*XX/5+3*XX/5)/2-20, (YY/3+YY/3+2*YY/5/3)/2)   << text("INDIT")
            <<color(255, 255,   0) << move_to((2*XX/5+3*XX/5)/2-20, (YY/3+3*YY/5/3+2*YY/3)/2) << text("KILEP");

        if(ev.button==btn_left && ev.pos_x>=2*XX/5 && ev.pos_x<=3*XX/5 || ev.keycode==key_space)
        {
            if(ev.pos_y>=YY/3 && ev.pos_y<=YY/3+2*YY/5/3 || ev.keycode==key_space)
            {
                flappy f;
                bool b=false;
                int evszamlalo=1;
                int rszoras=100;
                int rszel=100;
                int tav=2800;
                int r = rand()%700;
                bool menu=false;

                while(gin>>ev && !menu)
                {
                ///timer
                    if(ev.type==ev_timer && !f.g_o())
                    {
                        gout<<move_to(0,0)<<color(0,0,0)<<box(XX,YY);

                        f.rajzol();
                        f.mozog(0.013);
                        f.szamlalot_novel();
                    }
                ///repul
                    if(ev.keycode==key_space || b==true && !f.g_o())
                    {
                        f.repul(3.2);
                        b=true;
                    }
                    if(ev.keycode==-key_space) b=false;
                ///kapugeneralas
                    if(evszamlalo==(tav+r)/timer && !f.g_o())
                    {
                        f.kaput_general(YY/2-rszoras/2+rand()%rszoras, rszel+rand()%10);
                        evszamlalo=0;
                        rszoras+=15;
                        rszel-=3;
                        tav-=75;
                        r = rand()%700;
                    }
                ///Game Over
                    if(f.g_o())
                        {
                            gout<<color(255,0,0)<<move_to(XX/2-35, YY/2)<<text("Game Over");
                            gout<<color(170, 170, 250)<<move_to(2*XX/5, 4*YY/7)<<box_to(3*XX/5, 5*YY/7)
                                <<color(240, 50, 0)<<move_to((2*XX/5+3*XX/5)/2-17, (4*YY/7+5*YY/7)/2)<<text("MENU");
                            if(ev.button==btn_left && ev.pos_x>2*XX/5 && ev.pos_x<3*XX/5 && ev.pos_y>4*YY/7 && ev.pos_y<5*YY/7 || ev.keycode==key_space)
                            {
                                menu=true;
                                gout<<move_to(0,0)<<color(0,0,0)<<box(XX,YY);
                            }
                        }

                    evszamlalo++;
                    gout<<refresh;

                }

            }
        }
        gout<<refresh;
    }
    return 0;
}
